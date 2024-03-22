#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
pid_t fork( void );

int main() {
    pid_t child_proces;
    for(int i = 0;i<5;++i){
        child_proces=fork();
        if(child_proces!=0) {
            printf("Cześć\n");
        }else{
            break;
        }
        sleep(1);
    }
    return 0;

}
