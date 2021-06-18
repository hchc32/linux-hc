//select会循环检测条件,如果有满足条件的则执行并退出(有多个会随机公平的选一个执行),
//否则一直循环检测
package main


import "fmt"

func main(){
	var c1,c2,c3 chan int
	var i1,i2 int
	c2=make(chan int)
	//c3=make(chan int)
	// go func(){
	// 	c1<-100
	// }()
	go func(){
		c2<-20
	}()
	// go func(){
	// 	c3<-300
	// }()
	select{
	case i1 = <-c1:
		fmt.Printf("receive %d from c1\n",i1)
	case c2<-i2:
		fmt.Printf("send %d to c2\n",i2)
	case i3,ok := (<-c3):
		if ok{
			fmt.Printf("receive %d from c3\n",i3)
		}else{
			fmt.Printf("c3 is closed\n")
		}
	// default:
	// 	fmt.Printf("no communication\n")
	}

}