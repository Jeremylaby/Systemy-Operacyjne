
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define PIPE "./pipe"
double dx = 0.00001;
int n = 3;
int main(int argc,char*argv[]){
    if(argc<3){
        printf("Too few arguments");
        return 1;
    }
    if (mkfifo(PIPE, 0666) == -1) {
        printf("błąd \n");
        return -1;
    }
    printf("Main program is running...\n");
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    if(b<a){
        perror("Wrong arguments");
    }
    struct timespec start,end;
    clock_gettime(CLOCK_REALTIME,&start);
    char dx_str[20];
    char n_str[20];
    snprintf(dx_str, sizeof(dx_str), "%lf", dx);
    snprintf(n_str, sizeof(n_str) , "%d", n);
    if(fork()==0){
        execl("./calculate","calculate", dx_str, n_str, NULL);
        return 0;
    }
    int fd = open(PIPE,O_WRONLY);
    write(fd,&a, sizeof(double ));
    write(fd,&b, sizeof(double ));
    close(fd);
    double result=0.0;
    fd = open(PIPE,O_RDONLY);
    read(fd,&result,sizeof(double));
    close(fd);
    clock_gettime(CLOCK_REALTIME,&end);
    printf("Result: %lf in time: %ld ns, %lf s\n",result,end.tv_nsec-start.tv_nsec,(end.tv_nsec-start.tv_nsec)/1e9);
    unlink(PIPE);
    return 0;
}