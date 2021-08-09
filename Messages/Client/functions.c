#include "functions.h"

#define PORT 7981
#define MESS_SIZE 5

int message_counter;

void* thread_receive(void* client_sock){

    int client_socket = *(int*)client_sock;
    char recv_buffer[MESS_SIZE];
    int bytes;
    while(1) {
        bzero(recv_buffer, MESS_SIZE);
        bytes = recv(client_socket, recv_buffer, MESS_SIZE, 0);
        if (strcmp(recv_buffer, "end") == 0) { // second client sent all messages and server sends ending info 
            printf("\nSuccess - ending connection\n");
            exit(EXIT_SUCCESS);
        }
        if (bytes > 0) {
            printf("%s (%d bytes)\n\n", recv_buffer, bytes);
        }
    }
}

char* new_message() {
    srand(time(NULL));
    char* message = malloc(MESS_SIZE);

    int nr;
    int i = 0;
    while(i < MESS_SIZE) {
         nr = rand()%20 + 50;
         message[i++] = (char)nr;
    }
    return message;
}

// client sent all messages and this func is informing server about this
void say_goodbye(int client_sock) {
    char* bye = "bye";
    send(client_sock, bye, strlen(bye), 0);
}

void* thread_send(void* client_sock) {
    int client_socket = *(int*)client_sock;
    int current_message = 0;
    char send_buffer[MESS_SIZE];

    while(current_message < message_counter) {
        int bytes = 0;
        bzero(send_buffer, MESS_SIZE);
        char* message = new_message();
        strcpy(send_buffer, message);
        free(message);

        bytes = send(client_socket, send_buffer, strlen(send_buffer), 0);
            if (bytes > 0) {
            printf("\t %s  (%ld bytes) ---> \n", send_buffer, strlen(send_buffer));
            bzero(send_buffer, MESS_SIZE);
            current_message++;
        }
    sleep(2); // for showing conversation
    }
    say_goodbye(client_socket);
 }

void set_server(struct sockaddr_in* serverAddress) {
    memset(serverAddress, '\0', sizeof(serverAddress));
    serverAddress->sin_family = AF_INET;
    serverAddress->sin_port = htons(PORT);
    serverAddress->sin_addr.s_addr = inet_addr("127.0.0.1"); 
}