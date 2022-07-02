#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8000

int main(){
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
    
    printf("Connected to server\n");
    char str[1000];
    
    while(1){
        printf("Please enter the message to the server: ");
        char x; int it = 0;
        while((x = getchar()) != '\n'){
            str[it++] = x;
        }
        str[it] = '\0';

        write(client_sockfd, &str, sizeof(str));
        read(client_sockfd, &str, sizeof(str));
        printf("Server replied: %s\n", str);
    }
    close(client_sockfd);
}