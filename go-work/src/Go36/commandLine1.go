package main

import (
	"flag"
	"fmt"
	"os"
)

var name string

//自定义一个命令行容器 方式三
//var cmdLine = flag.NewFlagSet("question",flag.ExitOnError)

func init(){
	//方式2
	flag.CommandLine = flag.NewFlagSet("",flag.ExitOnError)
	flag.CommandLine.Usage = func() {
		fmt.Fprintf(os.Stderr,"Usage of %s:\n","question1")
		flag.PrintDefaults()
	}
	//方式三
	//cmdLine.StringVar(&name,"name","everyone","The greeting object.");
	flag.StringVar(&name,"name","everyone","The greeting object.")
}

func main(){
	//方式１
	// flag.Usage = func(){
	// 	fmt.Fprintf(os.Stderr,"Usage of %s:\n","question")
	// 	flag.PrintDefaults()
	// }
	
	//解析所有的命令行参数,方式三
	//cmdLine.Parse(os.Args[1:])
	flag.Parse()
	fmt.Printf("hello,%s\n",name);
}

