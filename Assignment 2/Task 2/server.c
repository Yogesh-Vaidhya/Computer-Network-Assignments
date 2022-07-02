#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>

#define LISTENQ 8
#define PORT 8000
struct timeval before, after;

double postfix(char* str);
double time_diff(struct timeval x , struct timeval y);

int main(){
    FILE* fp = fopen("server_records.txt" ,"w");
    fprintf(fp, "<Client Id> <Query> <Answer> <Time Elapsed>\n");
    fclose(fp);

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

    printf("Server Listening Request...\n");
    int client_addr_len = sizeof(client_addr), client_id = 0;
    char str[1000];

    while(1){
        int new_socket = accept(server_sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
        if(new_socket < 0){
            printf("Not able to connect to client socket\n");
            continue;
        }

        client_id += 1;
        if(fork() == 0){
            while(1){
                read(new_socket, &str, sizeof(str));
                gettimeofday(&before , NULL);
                double ans = postfix(str);
                gettimeofday(&after , NULL);
                double timeDiff = time_diff(before, after);
                
                FILE* fp = fopen("server_records.txt" ,"a");
                fprintf(fp, "<%d> <%s> <%0.3f> <%0.1f us>\n", client_id, str, ans, timeDiff);
                fclose(fp);

                snprintf(str, 1000, "%f", ans);
                write(new_socket, &str, sizeof(str));
            }
            close(new_socket);
        }
        close(new_socket);
    }
    close(server_sockfd);
}

double postfix(char* str){
    int len = strlen(str);
    int tp = -1;
    double stack[100];
    for(int i = 0; i < len; i++){
        if(str[i] == '+'){
            double val1 = stack[tp--];
            double val2 = stack[tp--];
            stack[++tp] = val1 + val2;
        }
        else if(str[i] == '-'){
            double val1 = stack[tp--];
            double val2 = stack[tp--];
            stack[++tp] = val2 - val1;
        }
        else if(str[i] == '*'){
            double val1 = stack[tp--];
            double val2 = stack[tp--];
            stack[++tp] = val1*val2;
        }
        else if(str[i] == '/'){
            double val1 = stack[tp--];
            double val2 = stack[tp--];
            stack[++tp] = val2/val1;
        }
        else if(str[i] != ' '){
            char tmp[100] = "";
            int iter = 0;
            while(str[i] != ' '){
                tmp[iter++] = str[i++];
            }
            stack[++tp] = strtod(tmp, NULL);
        }
    }
    return stack[tp];
}

double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;
    x_ms = 1000000*(double)x.tv_sec + (double)x.tv_usec;
    y_ms = 1000000*(double)y.tv_sec + (double)y.tv_usec;
    diff = (double)y_ms - (double)x_ms;
    return diff;
}