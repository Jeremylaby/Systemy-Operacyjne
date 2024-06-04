#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
static int global=0;
void *helloWorld(void *argc) {
    printf("Hello World %lu\n", pthread_self());
    while(1){
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex);
        global++;
        printf("Hello World %lu %d\n", pthread_self(),global);
        pthread_mutex_unlock(&mutex);
        sleep(1);
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}
int main(int argc, char *argv[]) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex,&attr);
    int n = atoi(argv[1]);
    pthread_t *threads = calloc(n, sizeof(pthread_t));
    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, helloWorld, NULL);
    }
    while(1);
    return 0;
}
