//
// Created by stani on 29.05.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;
int n=0;
void *helloWorld(void *argc) {
    pthread_mutex_lock(&mutex);
    while(n!=0){
        pthread_cond_wait(&cond, &mutex);
        printf("check\n");
    }
    printf("done\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}
int main(int argc, char *argv[]) {
    n=atoi(argv[1]);
    pthread_t *threads = calloc(n, sizeof(pthread_t));
    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, helloWorld, NULL);
    }

    while(n!=0) {
        pthread_mutex_lock(&mutex);
        n--;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

}
