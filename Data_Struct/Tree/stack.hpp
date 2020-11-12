#ifndef _STACK_H_
#define _STACK_H_

#include<iostream>
#include<cstdlib>
//#include"exp.hpp"
using namespace std;

template <class T>
class stack
{
	struct node
	{
		T data;
		struct node *next;
	};
public:
	stack();
	void push(T elem);
	void pop();
	T top();
	int size();
	bool empty();
private:
	node* first;
	node* last;
	int qsize;
};




//声明Ｔ是一个模板
template<class T>
bool stack<T>::empty() 
{
    if(first == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}
 
template<class T>
int stack<T>::size()
{
    return qsize;
}


template<class T>
stack<T>::stack()
{
    qsize = 0;

    first = NULL;
}

template<class T>
void stack<T>::push(T sta)
{
    node* pnew = new node;
    if(pnew == NULL)
    {
        cout << "new space error!" << endl;
        exit(0);
    }
    pnew->data = sta;
    pnew->next = first;
    first = pnew;
    qsize++;
    return ;
}

template<class T>
void stack<T>::pop()
{
    if(first == NULL)
    {
        cout << "pop error!" << endl;
        exit(0);
    }
    node* pdel = first;
    first = first->next;
    delete pdel;
    qsize--;
    return ;
}

template<class T>
T stack<T>::top()
{
    if(first == NULL)
    {
        cout << "get_top error!" << endl;
        exit(0);
    }
    return first->data;
}

#endif
