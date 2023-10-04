#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>
int main() {
    struct msg {
        long int m_type;
        char message[80];
    } myq;
    int key = ftok(".", 'a');
    int mqid = msgget(key, 0);
    printf("Enter message type: ");
    scanf("%ld", &myq.m_type);
    int ret = msgrcv(mqid, &myq, sizeof(myq.message), myq.m_type, IPC_NOWAIT|MSG_NOERROR);
    if (ret == -1)
        exit(-1);
    printf("Message type: %ld\n Message: %s\n", myq.m_type, myq.message);
}
