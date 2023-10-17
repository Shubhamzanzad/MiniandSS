/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write two programs so that both can communicate by FIFO -Use one way communication.
File name : 20b.c
Date : 12/10/2023
==================================================
*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(){
  char buff[50];
  int fd = open("FIFO1", O_RDONLY);
  if(read(fd, &buff, sizeof(buff)) == -1) {
    printf("FIFO error\n");
    return 1;
  }
  printf("Messge recieved: %s\n", buff);
return 0;
}
