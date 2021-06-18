package main

import "fmt"

func main(){
	//只获取函数的后两个返回值1
	_,numb,strs := number()
	fmt.Println(numb,strs)
}

func number()(int,int,string){
	a,b,c := 1,2,"str"
	return a,b,c
}