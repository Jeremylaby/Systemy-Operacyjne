#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *helloWorld(void *argc) {
    pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    sleep(1);
    printf("Hello World\n");
    sleep(100000);
    return NULL;
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    pthread_t threads[n];

    for (int i = 0; i < n; i++) {
        pthread_t newthread;
        pthread_create(&newthread, NULL, helloWorld, NULL);
        threads[i]=newthread;
    }


    printf("hej");

    for (int i = 0; i < 100; i++) {
        printf("hej\n");
    }
    for (int i = 0; i < n; i++) {
        pthread_cancel(threads[i]);
        sleep(1);
    }
    return 0;
}
