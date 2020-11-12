#include<iostream>
#include<cstdlib>
#include"stack.hpp"

using namespace std;

//二叉树模板
//结点类型定义
template<typename T>
struct TriNode
{
    //结点中的数据
    T data;
    int flag = 0;

    //左子树和右子树
    TriNode<T> *LeftChild , *RightChild;

    //无参数构造函数
    TriNode() : LeftChild(NULL) , RightChild(NULL) {}

    //有参数构造函数
    TriNode(T data , TriNode<T> *l = NULL ,TriNode<T> *r = NULL)
    { 
        this->data = data; 
        LeftChild = l; 
        RightChild = r; 
    }  
};

//二叉树类
template<typename T>
class BinaryTree
{
public:
    //构造函数
    BinaryTree()
    {
        this->pRoot = NULL;
    }

    //销毁二叉树表
    void DestoryBinaryTree() { _DestoryBinaryTree(pRoot); }

    //先序生成二叉树
    void PreOrderCreatChildTree(string a)
    {
        int i = 0;
        _PreOrderCreatChildTree(this->pRoot,a,i);
    }
    
    void PreAndInOrderCreatBinaryTree(string a,string b)
    {
        _PreAndInOrderCreatBinaryTree(this->pRoot,a,b);
    }

    void PreOrderTransferTree()
    {
        _PreOrderTransferTree(this->pRoot);
        cout << endl;
    }

    void InorderTransferTree()
    {
        _InorderTransferTree(this->pRoot);
    }

    void PostOrderTransferTree()
    {
        _PostOrderTransferTree(this->pRoot);
    }

    //非递归遍历二叉树
    void NotPreOrderTransferTree()
    {
        _NotPreOrderTransferTree(this->pRoot);
    }

     void NOtInorderTransferTree()
     {
         _NOtInorderTransferTree(this->pRoot);
     }

     void NotPOstOrderTransferTree()
     {
         _NotPOstOrderTransferTree(this->pRoot);
     }
    
    bool binaryTreeIsEmpty()
    {
        return pRoot == NULL;
    }

    int depth()
    {
        return _DepthTree(this->pRoot);
    }

    TriNode<T>* locate(T e)
    {
        return Location(pRoot,e);
    }

    TriNode<T>* Parent(TriNode<T>*& p)
    {
        return _Parent(this->pRoot,p);
    }



    int CountLeaf()
    {
        int n = 0;
        _CountLeaf(this->pRoot,n);
        return n;
    }

    //析构函数
    ~BinaryTree()
    {
        _DestoryBinaryTree(this->pRoot);
    }
    
    


private:
    //二叉树的根结点
    TriNode<T>* pRoot;

    //创建二叉树
    void _PreOrderCreatChildTree(TriNode<T> *&p,string& a,int &i)
    {
        //#是空指针的标识符
        if(a[i] == '#')
        {
            p = NULL;
        }
        else
        {
            p = new TriNode<T>(a[i]);
            _PreOrderCreatChildTree(p->LeftChild , a,++i);
            _PreOrderCreatChildTree(p->RightChild , a,++i);
            
        }
    }

    //二叉树的前序遍历
    void _PreOrderTransferTree(TriNode<T>* &p)
    {
        if(p)
        {
            cout << p->data << " ";
            _PreOrderTransferTree(p->LeftChild);
            _PreOrderTransferTree(p->RightChild);

        }
    }   

    //二叉树的前序非递归遍历
    void _NotPreOrderTransferTree(TriNode<T>* &p)
    {
        TriNode<T>* temp = p;
        stack<TriNode<T>*> st;
        while(!st.empty() || temp!=NULL)
        {
            while(temp!=NULL)
            {
                cout << temp->data << " ";
                st.push(temp);
                temp = temp->LeftChild;
            }
            if(!st.empty())
            {
                temp = st.top();
                st.pop();
                temp = temp->RightChild;
            }
        }
    }

    //二叉树的中序遍历
    void _InorderTransferTree(TriNode<T>*& p)
    {
        if(p)
        {
            _InorderTransferTree(p->LeftChild);
            cout << p->data << " ";
            _InorderTransferTree(p->RightChild);
        }
    }

    //二叉树的中序非递归遍历
    void _NOtInorderTransferTree(TriNode<T>*& p)
    {
        TriNode<T>* temp = p;
        stack<TriNode<T>*> s;
        while(!s.empty() || temp != NULL)
        {
            while(temp!=NULL)
            {
                s.push(temp);
                temp = temp->LeftChild;
            }
            if(!s.empty())
            {
                temp = s.top();
                cout << temp->data << " ";
                s.pop();
                temp = temp->RightChild;
            }

        }
    }

    //二叉树的后序遍历
    void _PostOrderTransferTree(TriNode<T>*& p)
    {
        if(p)
        {
            _PostOrderTransferTree(p->LeftChild);
            _PostOrderTransferTree(p->RightChild);
            cout << p->data << " ";
        }
    }

    //二叉树的后序非递归遍历
    void _NotPOstOrderTransferTree(TriNode<T>*& p)
    {
        stack<TriNode<T>*> s;
        TriNode<T>* temp = p;
        while(!s.empty() || temp!=NULL)
        {
            //循环找到最左边的点
            while(temp!=NULL)
            {
                //标志该结点走过一次
                temp->flag = 1;
                s.push(temp);
                temp = temp->LeftChild;
            }

            temp = s.top();
            
            //标志该结点走过一次
            if(temp->flag == 1)
            {
                temp->flag = 2;
                temp = temp->RightChild;
            }
            //标志该结点走过两次,应该pop()
            else if(temp->flag == 2)
            {
                cout << temp->data << " ";
                s.pop();
                //跳出上面的while循环
                temp = NULL;
            }
        }
    }
    

    //递归销毁二叉树
   void _DestoryBinaryTree(TriNode<T>*& p)
   {
       if(p)
       {
            _DestoryBinaryTree(p->LeftChild);
            _DestoryBinaryTree(p->RightChild);
            delete p;
       }
       p = NULL;
   }

   //根据中序和前序创建二叉树
    void _PreAndInOrderCreatBinaryTree(TriNode<T>*& p ,string preorder ,string inorder)
    {
        //没有左子树或者右子树
        if(inorder.size() == 0)
        {
            p = NULL;
        }
        else
        {
            int mid = 0;
            p = new TriNode<T>(preorder[0]);
            if(p == NULL)
            {
                cout << "malloc error !" << endl;
            }
            
            for(int i = 0; i < inorder.size() ; i++)
            {
                if(inorder[i] == preorder[0])
                {
                    mid = i;
                }
            }
            //递归建立左子树
            //mid-1 ~ mid
            _PreAndInOrderCreatBinaryTree(p->LeftChild,preorder.substr(1,mid),inorder.substr(0,mid-1));
            //递归建立右子树
            _PreAndInOrderCreatBinaryTree(p->RightChild,preorder.substr(mid+1),inorder.substr(mid+1));
        }
    }
    
    //求二叉树的深度
    int _DepthTree(TriNode<T>*& p)
    {
        if(p == NULL)
        {
            return 0;
        }
        //递归求左子树的深度
        int HLeft = _DepthTree(p->LeftChild);
        //递归求右子树的深度
        int HRight = _DepthTree(p->RightChild); 
        return HLeft > HRight ? HLeft+1 : HRight+1;
    }

    //寻找值为find_data的位置
    TriNode<T>* Location(TriNode<T>*& p , T& find_data)
    {
        if(p == NULL || p->data == find_data)
        {
            return p;
        }
        TriNode<T>* q = NULL;
        q = Location(p->LeftChild , find_data);
        if(q)
        {
            return q;     
        }
        q = Location(p->RightChild , find_data);
        return q;
    }

    //求结点的双亲
    TriNode<T>* Parent(TriNode<T>*& p, TriNode<T>*& q)
    {
        //根结点没有双亲
        if(p == NULL || p == q)
        {
            return NULL;
        }
        //递归结束条件
        if(p->LeftChild == q || p->RightChild == q)
        {
            return p;
        }
        TriNode<T>* temp = NULL;
        temp = Parent(p->LeftChild , q);
        if(temp == q)
        {
            return q;
        }
        temp = Parent(p->RightChild , q);
        return q;
    }

    void _CountLeaf(TriNode<T>*& p , int& n)
    {
        if(!p)
        {
            n=0;
            return;
        }
        CountLeaf(p->LeftChild,n);
        CountLeaf(p->RightChild,n);
        if(!p->LeftChild && !p->RightChild)
        {
            n++;
        }
    }
};



int main()
{
    BinaryTree<char> tree1;
    //创建二叉树
    //tree1.PreOrderCreatChildTree("-+a##*b##-c##d##/e##f##");
    tree1.PreOrderCreatChildTree("abd#g###ce#h##f##");
    //判断二叉树是否为空
    if(tree1.binaryTreeIsEmpty())
    {
        cout << "BinaryTree is empty" << endl;
    }
    else
    {
        cout << "BinaryTree is not empty" << endl;
    }
    //打印二叉树的深度
    cout << tree1.depth() << endl;

    //二叉树的前序遍历
    cout << "前序递归遍历" << endl;
    tree1.PreOrderTransferTree(); 
    //非递归
    cout << "前序列非递归遍历" << endl;
    tree1.NotPreOrderTransferTree();
    cout << endl;
    //二叉树的中序遍历
    cout << "中序递归遍历" << endl;
    tree1.InorderTransferTree();
    cout <<endl;
    //getchar();
    cout << "中序非递归遍历" <<endl;
    tree1.NOtInorderTransferTree();
    cout <<endl;
    //二叉树的后序遍历
    cout << "后序递归遍历" <<endl;
    tree1.PostOrderTransferTree();
    cout << endl;
    cout << "后序非递归遍历" <<endl;
    tree1.NotPOstOrderTransferTree();
    cout <<endl;
    
    //获取二叉树某一数据的位置
    TriNode<char> *p = (tree1.locate('+'));
    cout << endl;
    cout << p->data << endl;

    return 0;
}
