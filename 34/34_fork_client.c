/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to create a concurrent server.
a. use fork
File name : 34_fork_client.c
Date : 12/10/2023
==================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(){
    int nsd;
    nsd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(6789); 
    serv.sin_addr.s_addr = INADDR_ANY;

    connect(nsd, (struct sockaddr *)&serv, sizeof(serv));

    char serv_res[1024];
    recv(nsd, serv_res,sizeof(serv_res),0);

    printf("Data from server: %s",serv_res);
    close(nsd);
    return 0;
}