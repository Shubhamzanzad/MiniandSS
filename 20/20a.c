/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write two programs so that both can communicate by FIFO -Use one way communication.
File name : 20a.c
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
  int fd = open("FIFO1", O_WRONLY);
  printf("Enter a msg: ");
  scanf("%[^\n]", buff);
  if(write(fd, buff, sizeof(buff)) == -1) {
    printf("FIFO error\n");
    return 1;
  }
  printf("msg wriiten\n");
return 0;
}
