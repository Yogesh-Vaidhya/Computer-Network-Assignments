#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage of client : %s [Server Port Number] [String]\n", argv[0]);
        exit(1);
    }

    int PORT = atoi(argv[1]);
    char str[1000];
    strcpy(str, argv[2]);
    
    int client_sockfd;
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sockfd < 0){
        printf("Unable to Create Socket\n");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connect(client_sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connet to the server\n");
        exit(1);
    }
    
    write(client_sockfd, &str, sizeof(str));
    read(client_sockfd, &str, sizeof(str));
    printf("Reversed String from server: %s\n", str);
    close(client_sockfd);
}