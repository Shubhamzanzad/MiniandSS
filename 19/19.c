#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<errno.h>
#include<stdio.h>
int main() {
    if (mknod("mknod_fifo_prog", S_IFIFO | 0666, 0) == 0) {
        printf("FIFO file created using mknod system call.\n");
    } 
    else {
        if(errno != EEXIST) {
            perror("mknod");
            return 1;
        }
    }

    if (mkfifo("mkfifo_fifo_prog", 0666) == 0) {
        printf("FIFO file created using mkfifo library function.\n");
    } 
    else {
        if(errno !=  EEXIST) {
            perror("mkfifo");
            return 2;
        }
    }
    return 0;
}
