//
// Created by stani on 22.03.2024.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
pid_t fork( void );
pid_t wait ( int *statloc );

int main(){
    pid_t child_proces;
    int i=10;
    double j=10.4;
    child_proces=fork();
    if(child_proces==0) {
        i=25;
        j=1.66666;
    }else{
        wait(NULL);
        printf("%d %f \n",i,j);
    }
    sleep(1);
    return 0;

}