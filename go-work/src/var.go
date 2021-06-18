package main

import "fmt"

//var _a string

//不可以在局部变量中出现
//implict:=1

//全局变量可以声明但是不使用
//局部变量必须使用
var (
	_a string
	_b int
	_c bool
)


func main(){
	//变量名后跟类型
	//var _a string = "hchc"
	var a string = "Runoob"
	//栈上对象也是０
	var stack_a string;
	fmt.Println(stack_a)
	fmt.Println(a)
	fmt.Println(_a)
	var b,c int = 1,2
	fmt.Println(b,c)

	//默认false
	var _bool bool;
	fmt.Println(_bool)

	//根据值去自行判定变量类型
	var d = true
	fmt.Println(d)

	//：＝左侧的变量不应该是已经被声明过的,不可以在局部变量中出现
	//var intVal uint8
	//初始化声明
	intVal := 1	//定义不适用通不过编译
	fmt.Println(intVal)

	f:="Runoob"
	fmt.Println(f)


}