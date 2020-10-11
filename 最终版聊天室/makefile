
A =  -I/usr/include/mariadb -L/usr/lib/x86_64-linux-gnu -lmariadbclient -lpthread -lm -ldl 
B = -pthread
all : server client 
.PTHONY : all

server : server.o my_sql.o
	gcc -o server server.o my_sql.o $(A) $(B)

my_sql.o : my_sql.c 
	gcc -c my_sql.c $(A) 

server.o : server.c 
	gcc -c server.c $(A) $(B)

client : client.o
	gcc -o client client.o  $(B)

client.o : client.c 
	gcc -c  client.c  $(B)



clean : 
	rm *.o