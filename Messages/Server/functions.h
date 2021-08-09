#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h> //EXIT_FAILURE
#include <string.h>
#include <strings.h> // strlen
#include <arpa/inet.h> //send, recv, socket, AF_INET, SOCK_STREAM
#include <unistd.h> // close
#include <pthread.h>

#define PORT 7981
#define BUFFER_SIZE 1024
#define TRUE 1
#define FALSE 0

int read_file(char* file_name, char* buffer);

void write_file(char* file_name, char* buffer);

void send_saved_msgs();

// when client sent all messages 
void stop_receiving(int client);

void pass_message(int client1, int client2);

// when all clients send message "bye" 
void end_connection();

int are_speaking();
void* conversation();

void* connect_client(void* servSoc);

int set_server();

#endif