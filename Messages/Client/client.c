#include "functions.h"

extern int message_counter;

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
