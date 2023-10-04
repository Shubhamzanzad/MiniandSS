#include<stdio.h>
#include<sys/ipc.h>
#include<string.h>
#include<sys/msg.h>
int main() {
    struct msg {
        long int m_type;
        char message[80];
    } myq;
    int key = ftok(".", 'a');
    int mqid = msgget(key, 0);
    printf("Enter message type: ");
    scanf("%ld", &myq.m_type);
    printf("Enter message text:");
    scanf("%[^\n]", myq.message);
    // size + 1 to accommodate terminating character
    msgsnd(mqid, &myq,strlen(myq.message) + 1, 0);
}
