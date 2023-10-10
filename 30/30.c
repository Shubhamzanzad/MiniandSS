#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main() {
    key_t key;
    int shmid;
    char *shmaddr;

    // Replace with the appropriate key (obtained from shared memory creation)
    key = ftok("/tmp", 'S');

    // Create the shared memory segment
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    // Attach the shared memory segment
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("shmat");
        return 2;
    }

    // Write some data to the shared memory
    strcpy(shmaddr, "Hello, shared memory!");

    // Print the data in shared memory
    printf("Data in shared memory: %s\n", shmaddr);

    // Detach the shared memory segment
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        return 3;
    }

    // Re-attach with read-only access
    shmaddr = shmat(shmid, NULL, SHM_RDONLY);
    if (shmaddr == (char *)-1) {
        perror("shmat (read-only)");
        return 4;
    }
    
    // Try to overwrite (should fail since attached as read-only)
    printf("After this it will cause a segmentation fault because we trying to write in read-only file\n");
    printf("To avoid segmentation fault just comment the next line\n");
    strcpy(shmaddr, "Trying to overwrite");
    
    // Detach the shared memory segment (again)
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        return 5;
    }

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl (IPC_RMID)");
        return 6;
    }

    printf("Shared memory removed.\n");

    return 0;
}
