all: server_fun

server_fun: server.o functions.o
	gcc server.o functions.o -o server -lpthread

server.o: server.c
	gcc -c server.c

functions.o: functions.c
	gcc -c functions.c

clean:
	rm *.o
