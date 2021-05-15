#include<iostream>    
using namespace std;   
class X   
{   
public:   
 class Trouble {};   //注意：类中嵌套类的申明和定义，学习!!!    
  class small: public Trouble {};   
  class big:public Trouble {};//类中的继承!!!    
  void f(){   
  throw big();   
 }   
};   
int main()   
{   
  X x;   
  try{   
    x.f();   
  }   
  catch(X::small&)   
  {   
    cout<<"caught small"<<endl;  
 }   
  catch(X::big&)   
  {   
    cout<<"caught big"<<endl;   
  }   
  catch(X::Trouble &)   
  {   
    cout<<"caught Trouble"<<endl;   
  }   
   return 0;   
}   
