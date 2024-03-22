//
// Created by stani on 22.03.2024.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
pid_t fork( void );


int main(){
    pid_t child_proces;
    char *ar[10]= {"ping","google.com","-c10",NULL};
    child_proces=fork();
    if(child_proces==0) {
        execvp("./helloworld", ar);
    }else{
        printf("%d\n",(int)getpid());
    }
    sleep(1);
    return 0;

}