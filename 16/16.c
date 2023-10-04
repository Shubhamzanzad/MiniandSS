/*
===================================================
Name : Shubham Zanzad
Roll no. : MT2023040
Problem Statement : Write a program to send and receive data from parent to child vice versa. Use two way communication.
File name : 16.c
Date : 4/10/2023
==================================================
*/

#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/wait.h>

int main() {
  int fd0[2], fd1[2];
  char buf0[100], buf1[100];
  pipe(fd0);
  pipe(fd1);
  
  if(fork()) {
    close(fd0[1]);//close write end of child
    close(fd1[0]);//close read end of parent
    read(fd0[0], buf1, sizeof(buf1));
    printf("Msg recieved from child: %s\n", buf1);
    
    printf("Parents writes a msg: ");
    scanf("%[^\n]", buf0);
    write(fd1[1], buf0, sizeof(buf0));
  }

  else {
    close(fd0[0]);//close write end of parent
    close(fd1[1]);//close read end of child
    printf("Child writes a msg: ");
    scanf("%[^\n]", buf1);
    write(fd0[1], buf1, sizeof(buf1));
    
    read(fd1[0], buf0, sizeof(buf0));
    printf("Msg recieved from child: %s\n", buf0);
  }
  
return 0;
}
