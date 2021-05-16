//Hash Function是用来获得hash code的函数.
//容器中根据hash code放到指定的bucket中
#include <iostream>
#include <unordered_set>
using namespace std;

class Customer
{
public:
    string mFirstName;
    string mLastName;
    string mAge;
    Customer(string firstname,string lastname,string age):mFirstName(firstname),\
                                                          mLastName(lastname),\
                                                          mAge(age) {  }
    bool operator==(const Customer& c) const
    {
        return (mFirstName == c.mFirstName && mLastName == c.mLastName && mAge == c.mAge);
    }
};

class CustomerHash
{
public:
    size_t operator()(const Customer& c) const
    {
        return hash_val(c.mFirstName,c.mLastName,c.mAge);
    }

    template<typename... Types>
    size_t hash_val(const Types&... args)const
    {
        //seed为要返回的hash code
        size_t seed = 0;
        hash_value(seed,args...);
        return seed;
    }

    template<typename T,typename... Types>
    void hash_value(size_t& seed,const T& firstArg,const Types&... args) const
    {
        hash_combine(seed,firstArg);
        //递归拆包
        hash_value(seed,args...);
    }

    //递归结束条件
    template<typename T> 
      void hash_value(size_t& seed,const T& val) const   
      {
        hash_combine(seed,val);
      }

    template<typename T>
        void hash_combine(size_t& seed,const T& val) const
        {
            //黄金分割比例
            seed ^= hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
};

int main()
{
    unordered_multiset<Customer,CustomerHash> set;
    set.insert(Customer("a","b","1"));
    set.insert(Customer("c", "d", "2"));
    set.insert(Customer("e", "f", "3"));
    set.insert(Customer("g", "h", "4"));
     int myBucket_count = set.bucket_count();//返回有多少个篮子
    cout << set.bucket_count() << endl;

    CustomerHash hh;
    cout << "bucket postion of " << hh(Customer("a", "b", "1")) %myBucket_count << endl;//取余后 得出落在哪个篮子上
    cout << "bucket postion of " << hh(Customer("c", "d", "2")) %myBucket_count << endl;
    cout << "bucket postion of " << hh(Customer("e", "f", "3")) %myBucket_count << endl;
    cout << "bucket postion of " << hh(Customer("a", "b", "4")) %myBucket_count << endl;

    for (int i = 0; i< myBucket_count; i++)
    {
        //检测落在哪个篮子上
        cout << "bucket at #: " << i << "has: " << set.bucket_size(i) << endl;
    }
    return 0;
}

