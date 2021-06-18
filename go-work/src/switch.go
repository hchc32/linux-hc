package main

import "fmt"

func main(){
	var grade string = "B"
	var marks int  = 90

	switch marks{
		case 90:grade = "A"
		case 80:grade = "B"
		case 50,60,70:grade = "C"
		default:grade="D"
	}

	switch{
	case grade == "A":
		fmt.Printf("优秀!\n")
		fallthrough	//强制执行后面的case中的语句,不会判断case的值
	case grade == "B":
		fmt.Printf("666\n")
	default:
		fmt.Printf("差\n");
	}

	fmt.Printf("你的等级是%s\n",grade)

	var x interface{}

	switch i:=x.(type){
	case nil:
		fmt.Printf("x的类型是:%T\n",i)
	case int:
		fmt.Printf("x的类型是%T\n",i)
	default:
		fmt.Printf("不知道!\n")
	}
}