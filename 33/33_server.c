/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to communicate between two machines using socket.
File name : 33_server.c
Date : 12/10/2023
==================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/socket.h>
#include <arpa/inet.h>

int main() {
    struct sockaddr_in serv, cli;
    int sd, nsd;
    char buf[50];

    // sd = Socket Descriptor
    sd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;

    // Specify destination address of socket
    // INADDR_ANY = Automatically assign IP address (of current machine)

    serv.sin_addr.s_addr = INADDR_ANY;

    // Meaning of htons() will be explained later
    serv.sin_port = htons(3558);

    // sd has become the server now
    bind(sd, (void*) (&serv), sizeof(serv));
    listen(sd, 5); // 5 = size of wait queue for connections
    int sz = sizeof(cli);

    // Accept connection from client and put its details in cli
    // Server will wait at this line till a client connects with proper IP address and port number

    // nsd = New Socket Descriptor
    nsd = accept(sd, (void*) (&cli), &sz);

    // Read data sent by cli (nsd) and store it in buf
    read(nsd, buf, sizeof(buf));
    printf("Message from client: %s\n", buf);
    write(nsd, "ACK from Server\n", 17);

    return 0;
}
