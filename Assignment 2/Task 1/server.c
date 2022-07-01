#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define LISTENQ 8

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Usage of client : %s [Server Port Number]\n", argv[0]);
        exit(1);
    }

    int PORT = atoi(argv[1]);
    char str[1000];
    
    int server_sockfd;
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sockfd < 0){
        printf("Unable to Create Socket\n");
        exit(1);
    }

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(server_sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connet to the server\n");
        exit(1);
    }
    
    if(listen(server_sockfd, LISTENQ) < 0){
        printf("Unable to listen\n");
        exit(1);
    }

    int client_addr_len = sizeof(client_addr);

    while(1){
        printf("Server Listening Request...\n");
        int new_socket = accept(server_sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
        if(new_socket < 0){
            printf("Not able to connect to client socket\n");
            continue;
        }
        read(new_socket, &str, sizeof(str));

        int len = strlen(str);
        printf("String sent by the client : %s\n", str);
        for (int i = 0, j = len - 1; i < j; i++, j--) { 
            char temp = str[i]; 
            str[i] = str[j]; 
            str[j] = temp; 
        }
        write(new_socket, &str, sizeof(str));
        printf("Reversed String is sent to the client\n");
        printf("-------------------\n");
        close(new_socket);
    }
    close(server_sockfd);
}