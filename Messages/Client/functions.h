#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h> //rand
#include <string.h> // strcpy, strlen, memset
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <pthread.h>

#define PORT 7981
#define BUFFER_SIZE 1024
#define NUMB_OF_MESSAGESS 8

void* thread_receive(void* client_sock);

// client sent all messages and this func is informing server about this
void say_goodbye(int client_sock);

void* thread_send(void* client_sock);

void set_server(struct sockaddr_in* serverAddress) ;

#endif