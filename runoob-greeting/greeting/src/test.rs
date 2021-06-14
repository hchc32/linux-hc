fn main()
{
    let a = 12;
    println!("a is {}",a); 
    println!("a is {},a again is {}",a,a);   
    //可变参数当做数组来访问
    println!("a is {0},a again is {0}",a);
    //转义字符
    println!("{{}}");
    
    // let mut a = 123;
    // a = 456;

    // let a = 123;
    // let a = 456;

    //重影是指用同一个名字重新代表另一个变量实体,其类型、可变属性和值都可以变化
    //但可变变量赋值仅能发生值的变化。
    let x = 5;
    let x = x+1;
    let x = x*2;
    let x = "hchc";
    println!("The Value of x is : {}",x);
    
    //!Error:let mut s = "123";
    //s = s.len();
    //println!("The s is {}",s);


}