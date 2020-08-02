
A =  -I/usr/include/mariadb -L/usr/lib/x86_64-linux-gnu -lmariadbclient -lpthread -lm -ldl 

all : server client
.PTHONY : all

server : server.o my_sql.o
	gcc -o server server.o my_sql.o $(A)

my_sql.o : my_sql.c 
	gcc -c my_sql.c $(A)

server.o : server.c 
	gcc -c server.c $(A)

client : client.o
	gcc -o client client.o $(A)

client.o : client.c 
	gcc -c  client.c $(A)

clean : 
	rm *.o