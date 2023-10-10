#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
int main() {
    int fd = open("myfifo", O_WRONLY);
    char buff[50];
    printf("Enter a msg\n");
    scanf("%[^\n]", buff);
    write(fd, &buff, sizeof(buff));
    return 0;
}