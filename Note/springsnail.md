#### spingsnail

1.getopt  optarg

2.xml

https://zhuanlan.zhihu.com/p/311711370



https://zhuanlan.zhihu.com/p/45173862



3.

```c++
/*syslog  等级
 * LOG_EMERG   0   紧急状态
 * LOG_ALERT   1   必须立即修复状态
 * LOG_CRIT    2   严重状态
 * LOG_ERR     3   出错状态
 * LOG_WARNING 4   警告状态
 * LOG_NOTICE  5   正常，但重要的状态
 * LOG_INFO    6   信息性消息
 * LOG_DEBUG   7   调试消息
*/

//ANSI C标准中几个标准预定义宏
// __FILE__ :在源文件中插入当前原文件名
// __LINE__ ：在源文件中插入当前源代码行号
// _DATE_: 在源文件插入当前的编译日期
// _TIME_: 在源文件中插入的编译时间
// _STDC_ : 当要求程序严格遵循ANSI C 标准时刻标示被赋值为1
// _cplusplus: 当编写C++程序时该标识符被定义
```

4.setup_sig_pipe()

> 1.创建ｅｐｏｌｌ红黑树根节点
>
> 2.创建sig_pipefd为通信套接字
>
> 3.设置写端为非阻塞，设置读端为非阻塞并且监听读端(epoll_ctl)
>
> 4.????????/

