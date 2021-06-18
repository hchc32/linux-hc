package main

import (
	"fmt"
	"unsafe"
)

//常量用作枚举
const (
	Unknown = 0
	Female = 1
	Male = 2
)

/*
常量可以用len(), cap(), unsafe.Sizeof()函数计算表达式的值。
常量表达式中，函数必须是内置函数，否则编译不过
*/
const (
	_a = "abc"
	_b = len(_a)
	_c = unsafe.Sizeof(_a) //why 16?
	//Ｇo中的string 类型是一个结构体类型
	/*type StringHeader struct{
		Data uintptr	//8
		Len int			//8
	}
	*/
)

//const中的特殊变量iota
//itoa用来统计当前const变量的个数
const (
	a = iota
	b
	c
	d = "ha"
	e
	f = 100
	g
	h = iota
	i
)

const (
	_i=1<<iota
	_j=3<<iota
	_k
	_l
)




func main(){
	const LENGTH int  = 10
	const WIDTH int = 5
	var area int
	//const a,b,c = 1,false,"str"

	area = LENGTH*WIDTH
	fmt.Printf("面积为:%d",area)
	println()
	println(a,b,c,d,e,f,g,h,i)
	println(_i,_j,_k,_l)


}