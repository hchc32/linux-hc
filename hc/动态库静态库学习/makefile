ScFiles=$(wildcard *.c)

app:work.o member.o user.o
	gcc -o app work.o member.o user.o

main.o:work.c
	gcc -c work.c

member.o:member.h
	gcc -c member.h
	
user.o:user.h
	gcc -c user.h

test:
	echo $(SrcFiles)
