#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

//双字节对齐
#define WSIZE            8
//单字节对齐
#define DSIZE            4

#define MAX(x,y) ((x) > (y) ? (x) : (y))
//扩展堆时默认的扩展大小
#define CHUNKSIZE        (1<<8)
//模拟虚拟内存大小
#define MAX_HEAP         (1<<12)

//对边界的计算
#define PACK(size,alloc) ((size) | (alloc))

#define GET(p)           (*(unsigned int *)(p))
#define PUT(p,val)       (*(unsigned int *)(p) = (val))

//计算size位和标志位
#define GET_SIZE(p)      (GET(p) & ~0x7)
#define GET_ALLOC(p)     (GET(p) & 0x1)

//通过当前指针位置计算头尾标记起始位置
#define HDRP(bp)         ((char*)(bp) - WSIZE)
#define FTRP(bp)         ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

//计算next块的 和prev块的bp值
#define NEXT_BLKP(bp) ((char*)bp + GET_SIZE((char*)bp - WSIZE))
#define PREV_BLKP(bp) ((char*)bp - GET_SIZE((char*)bp - DSIZE))

//保存堆顶
static char *mem_heap;
//mem_heap与mem_brk之间的空间代表已分配的虚拟内存,brk和max_addr代表未分配的
static char *mem_brk;
//最大模拟虚拟内存地址
static char *mem_max_addr;
//序言块与结尾块之间靠近序言块那一侧的指针
void *heap_listp;
//下一次适配的全局变量
void *next_listp;

//初始化虚拟内存空间
void men_init();
//初始化堆空间
int mm_init();
//扩展堆的大小(max)
void *mem_sbrk(int incr);
//用一个新的空闲块扩展堆
void *extend_heap(size_t word);
//合并块
void* coalesce(void* bp);
void *place(void *bp , size_t size);


//初始化虚拟空间
void men_init()
{
    mem_heap     = (char*)malloc(MAX_HEAP);
    mem_brk      = (char*)mem_heap;
    mem_max_addr = (char*)(mem_heap+MAX_HEAP);
}

//扩展堆的大小
void *mem_sbrk(int incr)
{
    char* old_brk = mem_brk;
    
    //扩展大小太大或者太小
    if(incr < 0 || ((mem_brk + incr) > mem_max_addr))
    {
        errno = ENOMEM;
        perror("error: run out of memory!\n");
        return (void*)-1;
    }

    mem_brk = mem_brk + incr;
    return (void*)old_brk;
}

//这里是有头有尾的数组 头：序言块　　尾：结尾块
//初始化堆空间
int mm_init()
{
    //虚拟内存不够
    if((heap_listp = mem_sbrk(4*WSIZE)) == (void*)-1)
    {
        return -1;
    }
    PUT(heap_listp , 0);
    PUT(heap_listp+WSIZE , PACK(DSIZE , 1));
    PUT(heap_listp + 2*WSIZE , PACK(DSIZE , 1));
    //设置结尾块,size位为０，存在位为1　
    PUT(heap_listp + 3*WSIZE , PACK(0 , 1));

    heap_listp += (2*WSIZE);
    next_listp = heap_listp;
    
    if(extend_heap(CHUNKSIZE/WSIZE) == NULL) return -1;

}

//一个字四个字节 
void *extend_heap(size_t word)
{
    char *bp;
    size_t size;
    //字节对齐
    size = (word%2)?((word+1)*WSIZE) : (word*WSIZE);
    //当请求的内存大于虚拟内存的时候，返回ＮＵＬＬ
    if((long)(bp = (char*)mem_sbrk(size)) == -1) return NULL;
    
    //代表大小为size的空闲块
    //初始化头部
    PUT(HDRP(bp),PACK(size,0));
    //初始化尾部
    PUT(FTRP(bp),PACK(size,0));
    //更新结束块
    PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1));
    //从bp位置可以开始分配内存，把内存块的后面与新分配的内存进行合并
    return coalesce(bp);
}

//释放堆空间
void mm_free(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));

    //合并块
    coalesce(bp);
}

//合并块
void* coalesce(void* bp)
{
    //获取前一个块的标志位
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    //获取后一个块的标志位
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    //获取当前的size
    size_t size = GET_SIZE(HDRP(bp));
    
    //对四种情况进行讨论
    //1.前后都有已分配块
    if(prev_alloc && next_alloc)
    {
        return bp;
    }
    //2.前为未分配块，后为已分配块、
    else if(!prev_alloc && next_alloc)
    {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        //更新尾部信息
        PUT(FTRP(bp),PACK(size,0));
        //更新新的头部信息
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        //更新指针指向
        bp = PREV_BLKP(bp);
    }
    //3.前为已分配块，后为未分配块
    else if(prev_alloc && !next_alloc)
    {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        //更新头部信息，尾部信息
        PUT(HDRP(bp),PACK(size,0));
        PUT(FTRP(bp),PACK(size,0));
    }
    //4.前后均为未分配块
    else
    {
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
        //更新头部信息
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        //更新尾部信息
        PUT(FTRP(NEXT_BLKP(bp)),PACK(size,0));
        bp = PREV_BLKP(bp);
    }
    return bp;
}

//放置策略为首次适配，从heap_listp开始寻找，而不是从mem_heap开始
void *find_fit(size_t size)
{
    void *bp;
    for(bp = heap_listp ; GET_SIZE(HDRP(bp)) != 0 ; bp = NEXT_BLKP(bp))
    {
        //找到匹配
        if(!GET_ALLOC(HDRP(bp)) && GET_SIZE(HDRP(bp))>=size )
        {
            return bp;
        }
    }
    return NULL;
}

void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsizes;
    void *bp;
    if(size == 0)
    {
        return NULL;
    }
    else if(size < 2*DSIZE)
    {
        asize = 2*DSIZE;
    }
    else
    {
        //进行字节对齐，加上两个标志位，一个为wsize
        asize = DSIZE*((size+DSIZE+DSIZE-1)/DSIZE);
    }

    //命中
    if((bp = find_fit(asize)) != NULL)
    {
        place(bp,asize);
        next_listp = NEXT_BLKP(HDRP(bp));
        return bp;
    }

    //不命中
    extendsizes = MAX(asize , CHUNKSIZE);
    //虚拟内存已经消耗殆尽了
    if((bp = extend_heap(extendsizes/DSIZE)) == NULL)
    {
        return NULL;
    }
    //内存替换
    place(bp,asize);
    next_listp = NEXT_BLKP(HDRP(bp));
    return bp;
}

void *place(void *bp , size_t size)
{
    size_t asize = GET_SIZE(HDRP(bp));
    //大于最小块,进行内存分离
    if((asize-size) >= 2*DSIZE)
    {
        PUT(HDRP(bp),PACK(size,1));
        PUT(FTRP(bp),PACK(size,1));
        *(FTRP(bp)-1)='\0';
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp),PACK(asize-size,0));
        PUT(FTRP(bp),PACK(asize-size,0));
    }
    else
    {
        PUT(HDRP(bp),PACK(asize,1));
        PUT(FTRP(bp),PACK(asize,1));
        *(FTRP(bp)-1) = '\0';
    }
}


int main()
{
    men_init(); 
    mm_init();
    char *temp = (char*)mm_malloc(sizeof(char)*256);
    strcpy(temp,"hello world!");
    strcat(temp,"\0");
    printf(":%s\n",temp);
    size_t bbb = GET_ALLOC(HDRP(temp));
    mm_free(temp);
    size_t aaa = GET_ALLOC(HDRP(temp));
    printf("%s\nend\n",temp);
    free(mem_heap);
    return 0;
}

