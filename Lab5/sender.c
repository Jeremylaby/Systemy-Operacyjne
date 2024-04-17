//
// Created by stani on 11.04.2024.
//
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
double f(int x){
    return 4/(x*x+1);
}
double calculate_integral(double a,int n,double dx){
    double integral=0.0;
    for(int i=0;i<n;i++){
        integral+=f(a+i*dx)*dx;
    }
    return integral;
}
int main(int argc,char*argv[]){
    if(argc<3){
        printf("Too few arguments");
        return 1;
    }
    double dx = atof(argv[1]);
    int n = atoi(argv[2]);
    int m = round(1/dx);
    int fd[n][2];
    struct timespec start,end;
    clock_gettime(CLOCK_REALTIME,&start);
    for(int i=0;i<n;i++){
        pipe(fd[i]);
        close(fd[i][1]);
    }
    for(int i=0;i<n;i++){
        pid_t pid=fork();
        if(pid==0){
            close(fd[i][0]);
            int extra=i==0?m%n:0;
            double y = calculate_integral(i/n,m/n+extra,dx);
            write(fd[i][1],&y, sizeof(double ));
        }
    }
    while (wait(NULL) > 0);
    double dy=0.0;
    for(int i=0;i<n;i++){
        double tmp;
        read(fd[1][0],&tmp,sizeof(double ));
        dy+=tmp;
    }
    clock_gettime(CLOCK_REALTIME,&end);
    printf("Result: %lf in time: %ld\n",dy,start.tv_sec-end.tv_sec);
}