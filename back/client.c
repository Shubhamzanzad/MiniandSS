#include<arpa/inet.h>
#include<netinet/ip.h>
#include<signal.h>
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>

void LOG(char *msg) {
    printf("LOG: %s\n", msg);
}

void handler(int client_sd) {
    char read_buff[1024], write_buff[1024], temp_buff[1024];
    ssize_t read_size, write_size;
    do {
        bzero(read_buff, 1024);
        bzero(write_buff, 1024);
        bzero(temp_buff, 1024);

        read_size = read(client_sd, read_buff, sizeof(read_buff));
        if(read_size == -1) {
            perror("READ ERROR");
        }
        else if (read_size == 0) {
            printf("No data from server, closing the connection now");
        }
        else if (strchr(read_buff, '^') != NULL)
        {
            // Skip read from client
            strncpy(temp_buff, read_buff, strlen(read_buff) - 1);
            printf("%s\n", temp_buff);
            write_size = write(client_sd, "^", strlen("^"));
            if (write_size == -1)
            {
                perror("Error while writing to client socket!");
                break;
            }
        }
        else if(strchr (read_buff, '$') != NULL) {
            strncpy(temp_buff, read_buff, strlen(read_buff) - 2);
            printf("%s\n", temp_buff);
            printf("Closing the connection");
            break;
        }
        else {
            bzero(write_buff, 1024);
            if(strchr(read_buff, '#') != NULL)
                strcpy(write_buff, getpass(read_buff));
            else { 
                printf("%s\n", read_buff);
                scanf("%[^\n]%*c", write_buff);
            }
            write_size = write(client_sd, write_buff, 1024);
            if(write_size == -1) {
                perror("ERROR WHILE WRITING SOCKET");
                break;
            }
        }
    }while(read_size > 0);
    close(client_sd);
}       

int main() {
    signal(SIGINT, (void *) handler);
    int conn_status, client_sd;
    struct sockaddr_in server_addr;
    ssize_t msg_size;
    char buff[1024];

    client_sd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sd == -1) 
        perror("ERROR SOCKET CREATION!!!\n");
    LOG("Socket Created Successfully");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    conn_status = connect(client_sd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if(conn_status == -1)
        perror("FAILED TO CONNECT\n");
    LOG("Connected to server");
    // msg_size = read(client_sd, buff, 1024);
    // printf("%s", buff);
    // int ch;
    // printf("%s\n", buff);
    // scanf("%d", &ch);
    // send(client_sd, &ch, sizeof(ch), 0);
    handler(client_sd);
    close(client_sd);
}