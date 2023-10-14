#include<arpa/inet.h>
#include<netinet/ip.h>
#include<stdbool.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>

// #include "msg.h"
#include "administrator.h"
// #include "student.h"
// #include "professor.h"
// #include "admin.h"
// #include "stud.h"
// #include "prof.h"
//#include "structs.h"

void LOG(char *msg) {
    printf("%s\n", msg);
}

void start(int desc) {
    printf("Client is connected\n");
    ssize_t read_size, write_size;
    char read_buff[1024], write_buff[1024];
    int userChoice;
    write_size = write(desc, mess, sizeof(mess));
    if(write_size == -1) {
        perror("ERROR SENDING MSG");
    }
    else {
        bzero(read_buff, sizeof(read_buff));
        read_size = read(desc, read_buff, sizeof(read_buff));
        if (read_size == -1)
            perror("Error while reading from client");
        else if (read_size == 0)
            printf("No data was sent by the client");
        else
        {
            userChoice = atoi(read_buff);
            switch (userChoice)
            {
            case 1:
                // Admin
                admin_login_handler(desc);
                break;
            case 2:
                //prof_login_handler(desc);
                break;
            case 3:
                //stud_login_handler(desc);
                break;
            default:
                // Exit
                break;
            }
        }
    }
}

void main() {
    int server_sd, bind_status, listen_status, client_sd;
    struct sockaddr_in bind_addr;
    ssize_t msg_size;
    char buff[1024];

    server_sd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sd <= 0) {
        perror("ERROR COULDN'T CREATE SOCKET!!!\n");
    }
    LOG("Socket created successfully!");
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons(8081);
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    bind_status = bind(server_sd, (struct sockaddr *)&bind_addr, sizeof(bind_addr));
    if(bind_status == -1)
        perror("ERROR, BINDING ADDRESS!!!\n");
    else
        LOG("Socket bound successful.");

    listen_status = listen(server_sd, 20);
    if(listen_status == -1)
        perror("COULDN'T LISTEN\n");
    else
        LOG("Listening for communication request.");

    struct sockaddr_in client_addr;
    while(1) {
        socklen_t client_size = sizeof(client_addr);
        client_sd = accept(server_sd, (struct sockaddr *)&client_addr, &client_size);
        if(client_sd == -1){
            perror("ERROR WHILE CONNECTING");
            _exit(0);
        }
        else {
            if(!fork()) {
                start(client_sd);
                close(client_sd);
                _exit(0);        
            }
        }
    }
    close(server_sd);

}