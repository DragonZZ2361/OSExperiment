#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

const int N = 1e2 + 10;

int main(){
    int pid;
    int fd[2];
    char ReadPipeBuffer[N], WritePipeBuffer[N];

    pipe(fd);
    while((pid = fork()) == -1);

    if(pid == 0){
        read(fd[0], ReadPipeBuffer, 100);
        printf("Child Process Received : %s\n", ReadPipeBuffer);
        exit(0);
    }
    else{
        sprintf(WritePipeBuffer, "Child Process Id is %d from Parent Process Id %d\n", pid, getpid());
        write(fd[1], WritePipeBuffer, 100);
        wait(0);
        exit(0);
    }
}