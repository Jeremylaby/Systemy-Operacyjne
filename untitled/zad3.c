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
int sigfillset ( sigset_t* signal_set );
int sigprocmask(int how, const sigset_t *new_set, sigset_t *old_set);

int main(){
    pid_t child_proces;
    child_proces=fork();
    sleep(1);
    if(child_proces!=0){;
        printf("czesc");
        kill(1,SIGABRT);
        while(1){
            sleep(1);
        }
    }else{
        sigprocmask(SIG_BLOCK,&mask,NULL);
        while(1) {
            sleep(1);
            printf("czesc");
        }
    }
}