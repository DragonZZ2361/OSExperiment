#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#define MSGKEY 75

struct msgbuf{
    long mtype;
    char mtext[110];
};

void client(){
    int msgqid = msgget(MSGKEY, 0777|IPC_CREAT);
    for(int i = 10 ; i ; i--){
        struct msgbuf msg;
        msg.mtype = i;
        *(int *)msg.mtext = getpid();
        msgsnd(msgqid, &msg, sizeof(int), 0);
        printf("(client)(pid = %d) has sent message(type = %d) to Server \n", getpid(), (int)msg.mtype);
    }
    exit(0);
}

void server(){
    int msgqid = msgget(MSGKEY, 0777|IPC_CREAT);
    struct msgbuf msg;
    while(1){
        msgrcv(msgqid, &msg, sizeof(int), 0, 0);
        printf("(server)(pid = %d) received message(type = %d) from (client)(pid = %d) \n", getpid(), (int)msg.mtype, *(int *)msg.mtext);
        if(msg.mtype == 1){
            msgctl(msgqid, IPC_RMID, 0);
            exit(0);
        }
    }
}

int main(){
    int pid;
    while((pid = fork()) == -1);
    if(pid == 0){
        client();
    }
    else{
        server();
    }
}