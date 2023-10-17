/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to create a concurrent server.
a. use fork
File name : 34_fork_server.c
Date : 12/10/2023
==================================================
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

int main(){
    const char *msg = "Hello from fork server";
    int sd;
    sd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(6789);
    serv.sin_addr.s_addr = INADDR_ANY;

    bind(sd,(struct sockaddr*) &serv,sizeof(serv));

    listen(sd,5);
    while(1){
        if(!fork()){
            int cl;
            cl = accept(sd,NULL,NULL);
            send(cl,msg,sizeof(serv),0);
            close(cl);
        }
    }

    close(sd);
    return 0;
}