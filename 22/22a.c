#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
//#include<sys/time.h>

int main() {
    int fd;
    char buff[100];
    fd = open("myfifo", O_RDONLY);
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);
    
    // Set the timeout for 10 seconds
    struct timeval timeout;
    timeout.tv_sec = 10;
    printf("Waiting for data to be written into FIFO...\n");

    if(!select(fd + 1, &read_fds, NULL, NULL, &timeout)) {
      printf("No data\n");
    }
    else {
      printf("Data Available\n");
      read(fd, &buff, sizeof(buff));
      printf("%s\n", buff);
    }
    close(fd);
    return 0;
}
