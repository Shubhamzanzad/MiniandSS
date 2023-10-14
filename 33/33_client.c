#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    struct sockaddr_in serv;
    int sd;
    char buf[80];
    sd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    // Specify destination address of socket
    // INADDR_ANY = Automatically assign IP address (of current machine)
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(3558); // Must be same port as specified in server
    // Connect client socket sd to server
    connect(sd, (void*) (&serv), sizeof(serv));
    // Send message to server
    write(sd, "Hello Server\n", 14);
    read(sd, buf, sizeof(buf));
    printf("Message from server: %s\n", buf);
}