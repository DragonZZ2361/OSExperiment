#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSGKEY 75

struct msgbuf{
    long mtype;
    char mtext[110];
};

void client(){
    int msgqid = msgget(MSGKEY, 0777|IPC_CREAT);
    struct msgbuf msg;
    msg.mtype = 1;
    *(int *)msg.mtext = getpid();
    msgsnd(msgqid, &msg, sizeof(int), 0);
    msgrcv(msgqid, &msg, sizeof(int), 2, 0);
    printf("(client)(pid = %d) : receive reply from pid = %d \n", getpid(),*(int *)msg.mtext);
    exit(0);
}

void server(){
    int msgqid = msgget(MSGKEY, 0777|IPC_CREAT);
    struct msgbuf msg;
    msgrcv(msgqid, &msg, sizeof(int), 1, 0);
    printf("(server)(pid = %d) : serving for client pid = %d \n", getpid(),*(int *)msg.mtext);
    msg.mtype = 2;
    *(int *)msg.mtext = getpid();
    msgsnd(msgqid, &msg, sizeof(int), 0);
    wait(0);
    msgctl(msgqid, IPC_RMID, 0);
    exit(0);
}

int main(){
    int pid;
    pid = fork();
    if(pid == 0 ){
        client();
    }
    else{
        server();
    }
}