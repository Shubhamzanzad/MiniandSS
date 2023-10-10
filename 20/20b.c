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
