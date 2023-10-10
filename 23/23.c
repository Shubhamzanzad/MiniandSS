#include <stdio.h>
#include <unistd.h>

int main() {
    // Get the maximum number of files that can be opened
    long max_files = sysconf(_SC_OPEN_MAX);
    if (max_files == -1)
        perror("Failed to get maximum number of files");
    else
        printf("Maximum number of files that can be opened: %ld\n", max_files);

    // Get the size of a pipe's circular buffer
    long pipe_buffer_size = sysconf(_PC_PIPE_BUF);
    if (pipe_buffer_size == -1)
        perror("Failed to get pipe buffer size");
    else
        printf("Size of a pipe's circular buffer: %ld bytes\n", pipe_buffer_size);

    return 0;
}
