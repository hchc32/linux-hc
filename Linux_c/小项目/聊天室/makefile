

server:server.o my_error.o my_socket.o mysql.o user_password.o
	gcc -o server server.o my_error.o my_socket.o mysql.o user_password.o  `mysql_config --cflags --libs`  

client:client.o my_error.o my_socket.o user_password.o meun.o
	gcc -o client client.o my_error.o my_socket.o user_password.o meun.o mysql.o `mysql_config --cflags --libs`

client.o:client.c
	gcc -c client.c my_error.c meun.c mysql.o `mysql_config --cflags --libs`


meun.o:meun.c
	gcc -c meun.c 

server.o:server.c
	gcc -c server.c my_error.c `mysql_config --cflags --libs` 

my_error.o:my_error.c
	gcc -c my_error.c my_error.c `mysql_config --cflags --libs` 

my_socket.o:my_socket.c
	gcc -c my_socket.c  my_error.c `mysql_config --cflags --libs`

mysql.o:mysql.c
	gcc -c mysql.c  my_error.c `mysql_config --cflags --libs`
	
user_password.o:user_password.c
	gcc -c user_password.c my_error.c  `mysql_config --cflags --libs`

.PHONY: clean
clean:
	@rm -f *.o
