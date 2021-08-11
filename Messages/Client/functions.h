#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <pthread.h>


void* thread_receive(void* client_sock);

void say_goodbye(int client_sock);

void* thread_send(void* client_sock);

void set_server(struct sockaddr_in* serverAddress) ;

#endif