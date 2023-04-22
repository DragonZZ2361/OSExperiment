#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>

#define SHMKEY 75

void client(){
    int shmid = shmget(SHMKEY, 20, 0777|IPC_CREAT);
    void * addr = shmat(shmid, 0 ,0);
    while(1){
        if(*(int *)addr == -1){
            int number = rand() % 10;
            *(int *)addr = number;
            printf("(client)(pid = %d) has sent message(number = %d) to server !\n", getpid(), number);
        }
        else{
            sleep(1);
        }
        if(*(int *)addr == 0){
            break;
        }
    }
    shmdt(addr);
    exit(0);
}

void server(){
    int shmid = shmget(SHMKEY, 20, 0777|IPC_CREAT);
    void * addr = shmat(shmid, 0, 0);
    *(int *)addr = -1;
    while(1){
        if(*(int *)addr == -1){
            sleep(1);
        }
        else if(*(int *)addr == 0){
            break;
        }
        else{
            printf("(server)(pid = %d) has received message(number = %d) from (client) !\n", getpid(), *(int *)addr);
            *(int *)addr = -1;
        }
    }
    shmdt(addr);
    shmctl(shmid, IPC_RMID, 0);
    exit(0);
}

int main(){
    srand((unsigned int) time(NULL));
    int pid;
    while((pid = fork()) == -1);
    if(pid == 0){
        client();
    }
    else{
        server();
    }
}