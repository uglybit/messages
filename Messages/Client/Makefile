all: client_fun

client_fun: client.o functions.o
	gcc client.o functions.o -o client -lpthread

client.o: client.c
	gcc -c client.c

functions.o: functions.c
	gcc -c functions.c

clean:
	rm *.o
