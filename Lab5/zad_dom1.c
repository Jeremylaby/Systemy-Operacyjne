//
// Created by stani on 11.04.2024.
//
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
void handler(int signum){
    printf("Signal received %d\n",signum);
}
int main(int argc,char * argv[]) {
    if(argc!=2){
        printf("Too few arguments: %d\n",argc);
        return 1;
    }
    sigset_t mask;
    sigemptyset(&mask);
    if(strcmp(argv[1],"ignore")==0){
        signal(SIGUSR1,SIG_IGN);
    }else if(strcmp(argv[1],"handler")==0){
        signal(SIGUSR1,handler);
    }else if(strcmp(argv[1],"mask")==0){
        sigaddset(&mask, SIGUSR1);
        if(sigprocmask(SIG_SETMASK,&mask,NULL)==-1){
            perror("Failed to block all signals");
            return 2;
        }

        raise(SIGUSR1);
        sigset_t pending_signals;
        sigpending(&pending_signals);
        if(sigismember(&pending_signals,SIGUSR1)){
            printf("Signal is being masked\n");
        }else{
            printf("The signal is not masked\n");
        }
    }else if(strcmp(argv[1],"none")!=0){
        printf("Wrong argument\n");
        return 0;
    }
    raise(SIGUSR1);
    return 0;
}