#include <iostream>
#include<cstdlib>
#include"stack.hpp"
using namespace std;

#define Thread 1
#define Link   0

template<typename T>
struct ThreadNode
{
    //左右子数标志位
    int ltag,rtag;
    //左右孩子
    ThreadNode<T> *leftChild,*rightChild;
    //结点数据
    T data;
    ThreadNode(T u_data):data(u_data),ltag(0),rtag(0),leftChild(NULL),rightChild(NULL) {}
};


//前序递归创建二叉树
template<typename T>
void CreatPreTree(ThreadNode<T>* &root)
{
    T u_data;
    if(cin>>u_data)
    {
        if(u_data == '#')
        {
            root = NULL;
        }
        else
        {
            root = new ThreadNode<T>(u_data);
            if(root == NULL)
            {
                cout << "new error !" <<endl;
                exit(1);
            }
            CreatPreTree(root->leftChild);
            CreatPreTree(root->rightChild);
        }
     //   return ;
    }
}

//中序遍历线索化二叉树
template <typename T>
void InThreading(ThreadNode<T>* &t , ThreadNode<T>* &pre)
{
    ThreadNode<T>* root = t;
    if(root)
    {
        InThreading(root->leftChild,pre);
        //对左孩子空指针域置前驱线索
        if(!root->leftChild)
        {
            root->leftChild = pre;
            root->ltag = Thread;
        }
        //对右孩子空指针域置后继线索
        if(!pre->rightChild)
        {
            pre->rightChild = root;
            pre->rtag = Thread;
        }
        pre = root;
        InThreading(root->rightChild,pre);
    }
}

//中序列遍历线索二叉树
template <typename T>
void InOrderThread(ThreadNode<T>* &t)
{
    ThreadNode<T>* root = t;
    if(root == NULL)
    {
        return;
    }
    //寻找(头)结点
    while(root->ltag == Link)
    {
        root = root->leftChild;
    }
    int i  =10;
    while(i--)
    {
        cout << root->data << " ";
        if(root->rtag == Thread)
        {
            root = root->rightChild;
    //        cout << root->data << " ";
        }
        else
        {
            root = root->rightChild;
            while(root->ltag == Link)
            {
                root = root->leftChild;
            }
            cout << root->data << " ";
            root = root->rightChild;
        }
    }
}

template <typename T>
void _InorderTransferTree(ThreadNode<T>*& p)
   {
       if(p)
       {
           _InorderTransferTree(p->leftChild);
           cout << p->data << " ";
           _InorderTransferTree(p->rightChild);
       }
   }

template<typename T>
  //二叉树的后序遍历
void _PostOrderTransferTree(ThreadNode<T>*& p)
{
      if(p)
      {
          _PostOrderTransferTree(p->leftChild);
          _PostOrderTransferTree(p->rightChild);
          cout << p->data << " ";
      }
}

int main()
{
    ThreadNode<char> *pre;
    ThreadNode<char> *p;
    CreatPreTree(p);
    InThreading(p,pre);
    //InOrderThread(p);
    //cout << endl;
    /*
    _InorderTransferTree(p);
    cout<<endl;
    _PostOrderTransferTree(p);
    */
    return 0;
}
