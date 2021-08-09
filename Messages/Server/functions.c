#include "functions.h"

int clientCounter = 0;
int client_1 = -1;
int client_2 = -1; 
int client_1_sending = TRUE;
int client_2_sending = FALSE;


int read_file(char* file_name, char* buffer) {
    FILE *file;
    file = fopen(file_name, "r");
    if (!file) {
        return 0;
    }
    
    int get_from = 0;
    while(fgets(buffer + get_from, BUFFER_SIZE, file) != NULL) {
        get_from = strlen(buffer);
    }

    fclose(file);
    remove(file_name);
    return strlen(buffer);
}

void write_file(char* file_name, char* buffer) {
    int bytes;
    FILE* file = fopen(file_name, "a");
    if (!file) {
        perror("writing to file");
        exit(EXIT_FAILURE);
    }
    
    bytes = fputs(buffer, file);
    if (bytes > 0){
        fputc('\n', file);
    }

    bzero(buffer, BUFFER_SIZE);
    fclose(file);
}

void send_saved_msgs(){
    if (clientCounter < 2) { 
        return; // only one client 
    } 

    char* file_name = "queue.txt";
    char saved_msgs[BUFFER_SIZE];
    int bytes;

    if (read_file(file_name, saved_msgs) < 1) { 
        return; // no saved messages
    }
   
    bytes = send(client_2, saved_msgs, strlen(saved_msgs), 0);
    if (bytes > 0) {
        printf("Sent saved messages:\n%s\tto Client_2 (socket %d)\n", saved_msgs, client_2);
        bzero(saved_msgs, BUFFER_SIZE);
    }
}

// when client sent all messages 
void stop_receiving(int client) {
    client == client_1 ? (client_1_sending = FALSE) : (client_2_sending = FALSE);
}   

void pass_message(int client1, int client2) {
    char buffer[BUFFER_SIZE];
    int bytes = 0;

    bytes = recv(client1, buffer, BUFFER_SIZE, 0);
    if (bytes <= 0) {
        return;
    }

    if (strcmp(buffer, "bye") == 0) { // client sent all messages
        stop_receiving(client1); 
        bzero(buffer, BUFFER_SIZE);
        return;
    }
    printf("\nReceived: '%s' (socket %d)\n", buffer, client1);

    if (client2 > 0) {
        bytes = send(client2, buffer, strlen(buffer), 0);
        if (bytes > 0) {
            printf("Sent:  '%s' (socekt %d)\n", buffer, client2);
            bzero(buffer, BUFFER_SIZE);
            return;
        }    
    }

    printf("Client_2 unavailable, saving: '%s'\n", buffer);
    write_file("queue.txt", buffer); 
}

// when all clients send message "bye" 
void end_connection(){
    char* end = "end";
    send(client_1, end, strlen(end), 0);
    send(client_2, end, strlen(end), 0);
    close(client_1);
    close(client_2);
    printf("\nSuccess - ending connection\n");
    exit(EXIT_SUCCESS);
}

int are_speaking() {
    return client_1_sending || client_2_sending;
}

void* conversation(){
    while(TRUE) {
        are_speaking() ? "" : end_connection(); 
        send_saved_msgs(); 

        if (client_1_sending) {
            pass_message(client_1, client_2);
        }

        if (client_2_sending) {
            pass_message(client_2, client_1);
        }
    }
}

void* connect_client(void* servSoc){
    printf("Waiting for connection\n");
    struct sockaddr_in newAddress[2];
    socklen_t addrLength[2]; 
    int server_socket = *(int*)servSoc;

    while(clientCounter < 2){
        listen(server_socket, 2);
        
        switch(clientCounter) {
            case 0: client_1 = accept(server_socket, (struct sockaddr*)&newAddress[0], &addrLength[0]);
                    if (client_1 > 0) {
                        clientCounter++;                   
                        printf("Client_1 connected, socket: %d\n", client_1);
                    }
                    break;
            case 1: client_2 = accept(server_socket, (struct sockaddr*)&newAddress[1], &addrLength[1]);
                    if (client_2 > 0){
                        clientCounter++;
                        client_2_sending = TRUE;
                        printf("Client_2 connected, socket: %d\n", client_2);
                    }
                    break;
        }
    }
}

int set_server(){
    int server_socket;
    struct sockaddr_in serverAddress;
    server_socket = socket(AF_INET,SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("server socket\n");
        exit(EXIT_FAILURE);
    }

    int option = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    int binding = bind(server_socket, (struct sockaddr*)&serverAddress, 
            sizeof(serverAddress));

    if (binding != 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    return server_socket;
}