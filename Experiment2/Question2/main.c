#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid;
    pid = fork();
    if(pid < 0){
        printf("Fork Error !\n");
        exit(1);
    }
    else if(pid == 0){
        printf("Child Process %d is running .\n", getpid());
        setenv("PS1", "CHILD\\$", 1);
        printf("Child Process %d is calling exec .\n", getpid());
        if(execl("/bin/ls", "ls", "-l", NULL) < 0){
            printf("Child Process %d : Execl Error !\n", getpid());
            exit(1);
        }
        printf("Process %4d : You should never see this because the child is already gone .\n",getpid());
        printf("Process %4d : The child process is exiting .\n", getpid());
    }
    else{
        int stat_val;
        pid_t child_pid;
        child_pid = wait(&stat_val);
        printf("Child Process %d has finished .\n", child_pid);
        if(WIFEXITED(stat_val)){
            printf("Child Process %d exited with code %d .\n", child_pid, WEXITSTATUS(stat_val));
        }
        else{
            printf("Child Process %d terminated abnormally .\n", child_pid);
        }
    }

    exit(0);
}