#include <iostream>
#include<cstdlib>

using namespace std;

//AVL结点数据结构
template<typename T>
struct BSNode
{
    //构造函数
    BSNode(T t):key(t),lchild(nullptr),rchild(nullptr),parent(nullptr) {}
    BSNode() = default;

    //数据域
    T key;
    BSNode<T> *lchild;
    BSNode<T> *rchild;
    BSNode<T> *parent;
};

//AVL类
template<typename T>
class BSTree
{
public:
    BSTree():root(nullptr) {}
    ~BSTree() {}

    void Insert_Node(T key);
    //遍历
    void PreOrder() {preorder(root);}
    void InOrder() {inorder(root);}
    void PostOrder() {postorder(root);}
    //查找指定结点的前驱结点
    BSNode<T>* PreDecessor(BSNode<T>* x);
    //查找指定结点的后继结点
    BSNode<T>* Successor(BSNode<T>* x);
    //删除
    void Remove(T key);
private:
    BSNode<T> *root;
    //遍历
    void preorder(BSNode<T> *root);
    void inorder(BSNode<T> *root);
    void postorder(BSNode<T> *root);
    //删除
    void remove(BSNode<T> *pnode,T key);
};

//插入函数
template<typename T>
void BSTree<T>::Insert_Node(T key)
{
    BSNode<T> *pp = nullptr;
    BSNode<T> *p = root;
    if(p == nullptr)
    {
        p = new BSNode<T>(key);
        root = p;
        return;
    }
    while(p != nullptr)
    {
        pp = p;
        if(p->key > key)
        {
            p = p->lchild;
        }
        else if(p->key < key)
        {
            p = p->rchild;
        }
        else
        {
            break;
        }
    }
    p = new BSNode<T>(key);
    if(pp->key > key)
    {
        pp->lchild = p;
    }
    else
    {
        pp->rchild = p;
    }
    p->parent = pp ;    
}

template<typename T>
void BSTree<T>::preorder(BSNode<T>* root)
{
    if(root != NULL)
    {
        cout << root->key << " ";
        preorder(root->lchild);
        preorder(root->rchild);
    }
}

template<typename T>
void BSTree<T>::inorder(BSNode<T>* root)
{
    if(root != NULL)
    {
        inorder(root->lchild);
        cout << root->key << " ";
        inorder(root->rchild);
    }
}

template<typename T>
void BSTree<T>::postorder(BSNode<T> *root)
{
    if(root != NULL)
    {
        postorder(root->lchild);
        postorder(root->rchild);
        cout << root->key << " ";
    }
}

//寻找指定结点的前驱结点
//分为3种情况
template<typename T>
BSNode<T>* BSTree<T>::PreDecessor(BSNode<T>* x)
{
    //有左子树,找左子树中值最大的结点
    if(x->lchild != nullptr)
    {
        x = x->lchild;
        while(x->rchild != nullptr)
        {
            x = x->rchild;
        }
        return x;
    }
    //没有左子树
    //本身为左子树
    BSNode<T>* pp = x->parent;
    while(pp != nullptr && pp->lchild == x)
    {
        x = pp;
        pp = pp->parent;
    }
    //本身为右子树
    return pp;
}

//寻找指定结点的后继结点
template<typename T>
BSNode<T>* BSTree<T>::Successor(BSNode<T> * x)
{
    //有右子树
    if(x->rchild != nullptr)
    {
        x = x->rchild;
        while(x->lchild != nullptr)
        {
            x = x->lchild;
        }
        return x;
    }
    //没有右子树
    //是一个右孩子
    BSNode<T>* pp = x->parent;
    while(pp != nullptr && pp->rchild == x)
    {
        x = pp;
        pp = pp->parent;
    }
    //是一个左孩子
    return pp;
}

//删除
template<typename T>
void BSTree<T>::Remove(T key)
{
    remove(root,key);
}
template<typename T>
void BSTree<T>::remove(BSNode<T> *pnode,T key)
{
    if(pnode != nullptr)
    {
        if(pnode->value == key)
        {
            BSNode<T> *pdel = nullptr;
            if(pnode->lchild == nullptr || pnode->rchild == nullptr)
            {
                pdel = pnode;
            }
            else
            {
                pdel = PreDecessor(pnode);
            }

            //保存孩子指针
            BSNode<T>* pchild = nullptr;
            if(pdel->lchild != nullptr)
            {
                pchild = pdel->lchild;
            }
            else
            {
                pchild = pdel->rchild;
            }

            //孩子指向父父结点
            if(pchild != nullptr)
            {
                pchild->parent = pdel->parent;
            }
    
            //删除的是头结点
            if(pdel->parent == nullptr)
            {
                root = pchild;
            }
            //不是头结点
            else if(pdel->parent->lchild == pdel)
            {
                pdel->parent->lchild = pchild;
            }
            else
            {
                pdel->parent->rchild = pchild;
            }
            


        }

    }
}


int main()
{
    BSTree<int> tree;
    tree.Insert_Node(10);
    tree.Insert_Node(5);
    tree.Insert_Node(15);
    tree.Insert_Node(6);
    tree.Insert_Node(4);
    tree.Insert_Node(16);
    tree.PreOrder();
    cout << endl;
    tree.InOrder();
    cout << endl;
    tree.PostOrder();
    cout << endl;
    return 0;
}
