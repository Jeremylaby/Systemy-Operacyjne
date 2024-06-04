//
// Created by stani on 29.05.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2   = PTHREAD_COND_INITIALIZER;
int can_fly_on_vacation=1;
int available_reindeers=0;
void *reindeers(void *argc) {
    int id = *(int*)argc;
    while(1){
        int random_number = 5 + rand() % 6;
        printf("Reindeer %d going on vacations for: %d\n",id,random_number);
        sleep(random_number);
        printf("Reindeer %d returned from vacation\n",id);
        pthread_mutex_lock(&mutex);
        available_reindeers++;
        if (available_reindeers == 9) {
            pthread_cond_signal(&cond);
        }
        pthread_cond_broadcast(&cond);
        while (available_reindeers!=0){
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);

    }
}
void *santa_claus(void *argc) {
    while(1){
        printf("Santa is sleeping...\n");
        pthread_mutex_lock(&mutex);
        while (available_reindeers<9){
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        printf("Santa just woke up\n");
        int random_number = 2 + rand() % 3;
        printf("Santa is delivering presents %d\n",random_number);
        sleep(random_number);
        pthread_mutex_lock(&mutex);
        available_reindeers=0;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(int argc, char *argv[]) {
    pthread_t *threads = calloc(9, sizeof(pthread_t));
    pthread_t santa;
    pthread_create(&santa, NULL, santa_claus, NULL);
    int ids[9];
    for (int i = 0; i < 9; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, reindeers, &ids[i]);
    }

    while (1);
}