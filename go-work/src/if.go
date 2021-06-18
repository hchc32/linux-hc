package main

import "fmt"

func main(){
	a:=100
	b:=200

	if a==100{
		if b==200{
			fmt.Println("moki:a=100,b=200")
		}
	}else{
		fmt.Println("a!=100")
	}
	fmt.Printf("a=%d",a)


}