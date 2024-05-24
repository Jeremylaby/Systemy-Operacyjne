//
// Created by stani on 11.04.2024.
//
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
int main(int argc,char*argv[]){
    if(argc<2){
        printf("Too few arguments");
        return 1;
    }
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGUSR1);
    int pid = atoi(argv[1]);
    int mode = atoi(argv[2]);
    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    union sigval si;
    si.sival_int=mode;
    sigqueue(pid,SIGUSR1,si);
    sigsuspend(&mask);
}