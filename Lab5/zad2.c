//
// Created by stani on 05.04.2024.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
pid_t fork( void );
int kill(int pid, int SIGNAL);
int sigusr1=10;
int i =0;
void display(int signum){
    printf("czesc stan licznika: %d\n",i);
    i++;
    sleep(10);
}


int main(){
    pid_t child_proces;
    child_proces=fork();
    if(child_proces!=0){
        sleep(1);
        while(1){
            kill(child_proces,sigusr1);
        }
    }else{
        signal(sigusr1,display);
        while(1) {
            sleep(1);
        }
    }
}