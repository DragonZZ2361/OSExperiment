#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    pid_t pid1, pid2;
    pid1 = fork();
    if(pid1 < 0){
        printf("Fork Error !\n");
        exit(1);
    }
    else if(pid1 == 0){
        printf("First Child Process %d is running .\n", getpid());
        printf("b\n");
    }
    else{
        pid2 = fork();
        if(pid2 < 0){
            printf("Fork Error !\n");
            exit(1);
        }
        else if(pid2 == 0){
            printf("Second Child Process %d is running .\n", getpid());
            printf("c\n");
        }
        else{
            printf("Parent Process %d is running .Child Process: %d, %d \n", getpid(), pid1, pid2);
            printf("a\n");
        }
    }

    exit(0);
}