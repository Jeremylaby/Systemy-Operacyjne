//
// Created by stani on 29.03.2024.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
pid_t waitpid( pid_t pid, int *statloc, int options );
pid_t fork( void );
pid_t getppid(void);
int global = 10;
int main(int argc,char * argv[]) {
    if (argc < 2) {
        printf("Too few arguments. Usage: %s <number>\n", argv[0]);
        return 1;
    }
    pid_t child_proces;
    printf("Program name: %s\n",argv[0]);
    int local=5;
    child_proces=fork();
    if(child_proces==0){
        printf("Child process\n");
        local+=1;
        global+=1;
        printf("child's loacal = %d, child's global = %d\n",local,global);
        printf("Child pid = %d, Parent pid = %d\n",getpid(),getppid());
        return execl("/bin/ls","ls",argv[1],NULL);
    }else{
        int status;
        waitpid(child_proces,&status,0);
        printf("Parent proces\n");
        printf("Parent pid: %d, Child pid: %d\n",getpid(),child_proces);
        printf("Child exit code: %d\n",status);
        printf("parent's loacal = %d, parent's global = %d\n",local,global);
    }
    return 0;
}