#include "functions.h"

extern int clientCounter;
extern int client_1;
extern int client_2; 
extern int client_1_sending;
extern int client_2_sending;


int main() {
    int server_socket = set_server();
    pthread_t th_listen;
    pthread_t th_conversation;

    if (pthread_create(&th_listen, NULL, &connect_client, &server_socket) != 0) {
        printf("create th_listen\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&th_conversation, NULL, &conversation, NULL) != 0) {
        printf("create th_conversation\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(th_listen, NULL) != 0) {
        perror("join th_listen");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(th_conversation, NULL) != 0) {
        perror("join th_conversation");
        exit(EXIT_FAILURE);
    }

    return 0;
}