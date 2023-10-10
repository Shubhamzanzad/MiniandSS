#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main() {
    char buff1[50];
    char buff2[50];
    int fd1 = open("myfifo1", O_RDONLY);
    if(fd1 == -1) return 1;
    
    int fd2 = open("myfifo2", O_WRONLY);
    if(fd2 == -1) return 2;

    printf("Enter the text:");
    scanf(" %[^\n]", buff2);
    write(fd2, buff2, sizeof(buff2));
    
    read(fd1, buff1, sizeof(buff1));
    printf("The text from FIFO file: %s\n", buff1);
    return 0;
}