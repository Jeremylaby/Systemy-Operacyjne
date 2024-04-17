//
// Created by stani on 11.04.2024.
//
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
int modeChanges =0;
void handler(int signum,siginfo_t *info, void *context){
    int mode = info->si_value.sival_int;
    modeChanges+=1;
    switch (mode) {
        case 1:
            for(int i=1;i<=100;i++){
                printf("%d\n",i);
            }
            break;
        case 2:
            printf("%d\n",modeChanges);
            break;
        case 3:
            kill(info->si_pid,SIGUSR1);
            exit(1);
            break;


    }
    kill(info->si_pid,SIGUSR1);

}
int main(){
    printf("PID procesu: %d\n",getpid());
    struct sigaction act;
    act.sa_sigaction = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    while(1){
        sigsuspend(&act.sa_mask);
        printf("HI\n");
    }
}