#include <stdio.h>
#include <signal.h>

void obslugasygnalu(int signum){
    printf("cudowny sygnał\n");
}
int main() {

    signal(10,obslugasygnalu);
    raise(10);
    return 0;
}
