#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


pid_t fork( void );
pid_t getppid(void);
pid_t wait ( int *statloc );

int main(int argc,char * argv[]) {
    if (argc < 2) {
        printf("Too few arguments. Usage: %s <number>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    if (n == 0 && argv[1][0] != '0') {
        printf("Invalid argument. Please use an integer.\n");
        return 1;
    }
    pid_t child_proces;
    for(int i = 0;i<n;++i){
        child_proces=fork();
        if(child_proces==0) {
            printf("PPID: %d, PID: %d\n",getppid(),getpid());
            break;
        } else{
            wait(NULL);
        }
    }
    if(child_proces!=0) {

        printf("%d\n", n);
    }
    return 0;

}
