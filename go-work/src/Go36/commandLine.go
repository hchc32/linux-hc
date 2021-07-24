package main

import (
	"fmt"
	"flag"
)

var name string

func init(){
	flag.StringVar(&name,"name","everyone","The greeting object.");
}

func main(){
	init()
	flag.Parse()
	fmt.Printf("hello,%s\n",name);
}
