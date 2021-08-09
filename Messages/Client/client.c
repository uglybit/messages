#include <stdio.h>
#include <stdlib.h> //rand
#include <string.h> // strcpy, strlen, memset
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> // sleep
#include <pthread.h>

#define PORT 7981
#define BUFFER_SIZE 1024
#define NUMB_OF_MESSAGESS 8

int message_counter;
char messages[NUMB_OF_MESSAGESS][20] = {
    "Hi", "Ciao", "Hallo", "Auf wiedersehen", "Dzien dobry", "Czesc", "Servus", "Buongiorno"
    };

void* thread_receive(void* client_sock){

    int client_socket = *(int*)client_sock;
    char recv_buffer[BUFFER_SIZE];
    int bytes;
    while(1) {
        bzero(recv_buffer, BUFFER_SIZE);
        bytes = recv(client_socket, recv_buffer, BUFFER_SIZE, 0);
        if (strcmp(recv_buffer, "end") == 0) { // second client sent all messages and server sends info 
            printf("\nSuccess - ending connection\n");
            exit(EXIT_SUCCESS);
        }
        if (bytes > 0) {
            printf("%s (%d bytes)\n\n", recv_buffer, bytes);
        }
    }
}

// client sent all messages and this func is informing server about this
void say_goodbye(int client_sock) {
    char* bye = "bye";
    send(client_sock, bye, strlen(bye), 0);
}

void* thread_send(void* client_sock) {
    int client_socket = *(int*)client_sock;
    int current_message = 0;
    srand(time(NULL));
    char send_buffer[BUFFER_SIZE];

    while(current_message < message_counter) {
        int bytes = 0;
        bzero(send_buffer, BUFFER_SIZE);
        int index = rand() % NUMB_OF_MESSAGESS;

        strcpy(send_buffer, messages[index]);
        bytes = send(client_socket, send_buffer, strlen(send_buffer), 0);
            if (bytes > 0) {
            printf("\t %s  (%ld bytes) ---> \n", send_buffer, strlen(send_buffer));
            bzero(send_buffer, BUFFER_SIZE);
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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: clinet [number of messages]");
        exit(1);
    }
    message_counter = atoi(argv[1]);

    struct sockaddr_in serverAddress;  
    set_server(&serverAddress);
    
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(client_socket, (struct sockaddr*)(&serverAddress), sizeof(serverAddress)) != 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server\n");

    pthread_t sending, receiving;
    
    if (pthread_create(&receiving, NULL,&thread_receive, &client_socket) != 0) {
        printf("Problem with thread_2 creation\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&sending, NULL, &thread_send, &client_socket) != 0) {
    printf("Problem with thread_1 creation\n");
    exit(EXIT_FAILURE);
    }

    if (pthread_join(receiving, NULL) != 0) {
    printf("Problem with thread_2 join\n");
    exit(EXIT_FAILURE);
    }
    
    if (pthread_join(sending, NULL) != 0) {
        printf("Problem with thread_1 join\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
