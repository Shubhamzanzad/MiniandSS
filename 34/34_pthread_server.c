
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>

void *handle_client(void *arg) {
    int cl = *((int *)arg);
    const char *response = "Hemlo from p_thread server";
    send(cl, response, strlen(response), 0);
    close(cl);
    pthread_exit(NULL);
}

int main(){
    int sd;
    sd = socket(AF_INET,SOCK_STREAM,0);
    pthread_t thread_id;
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(5678);
    serv.sin_addr.s_addr = INADDR_ANY;

    bind(sd,(struct sockaddr*) &serv,sizeof(serv));

    listen(sd,5);
    while(1){
        int cl;
        cl = accept(sd,NULL,NULL);
        if(pthread_create(&thread_id, NULL, handle_client, &cl) != 0) {
            perror("Error in creating thread");
            close(cl);
        }
    }

    close(sd);
    return 0;
}