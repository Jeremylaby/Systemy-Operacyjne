#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/wait.h>


#define MAX_PRINT_TASKS 10
#define TEXT_SIZE 10
#define N 4
#define M 6
typedef struct {
    char text[TEXT_SIZE+1];
    int is_occupied;
}Task;
typedef  struct {
    Task tasks[MAX_PRINT_TASKS];
}TasksQueue;
sem_t *queue_occupied, *tasks_available, *tasks_ready;
TasksQueue *tasksQueue;
void init_semaphores();
void remove_semaphores();
void user_function(int user_id);
void printer_function(int printer_id);
int main() {
    remove_semaphores();
    init_semaphores();
    for(int i =0;i<N;i++){
        if(fork()==0){
            user_function(i);
            return 0;
        }
    }
    for(int i =0;i<M;i++){
        if(fork()==0){
            printer_function(i);
            return 0;
        }
    }
    for (int i = 0; i < N + M; i++) {
        wait(NULL);
    }
    remove_semaphores();
    return 0;
}
void init_semaphores(){
    int shm_fd = shm_open("/shm_tasksQueue",O_CREAT |O_RDWR,0666);
    ftruncate(shm_fd, sizeof(TasksQueue));
    tasksQueue= mmap(NULL,sizeof(TasksQueue),PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
    close(shm_fd);
    queue_occupied= sem_open("/sem_queue_occupied",O_CREAT,0666,1);
    tasks_available = sem_open("/sem_tasks_available",O_CREAT,0666,MAX_PRINT_TASKS);
    tasks_ready= sem_open("/sem_tasks_ready",O_CREAT,0666,0);
}
void remove_semaphores(){
    munmap(tasksQueue, sizeof(TasksQueue));
    shm_unlink("/shm_tasksQueue");
    sem_close(queue_occupied);
    sem_unlink("/sem_queue_occupied");
    sem_close(tasks_available);
    sem_unlink("/sem_tasks_available");
    sem_close(tasks_ready);
    sem_unlink("/sem_tasks_ready");
}
void user_function(int user_id){
    srand(time(NULL) ^ (getpid() << 16));
    while (1){
        char text[TEXT_SIZE+1];
        for(int i=0;i<TEXT_SIZE;i++){
            text[i]='a'+rand()%26;
        }
        sem_wait(queue_occupied);
        sem_wait(tasks_available);
        for(int i=0;i<MAX_PRINT_TASKS;i++){
            if(!tasksQueue->tasks[i].is_occupied){
                strcpy(tasksQueue->tasks[i].text,text);
                tasksQueue->tasks[i].is_occupied=1;
                printf("User %d send message %s\n",user_id,text);
                break;
            }
        }
        sem_post(queue_occupied);
        sem_post(tasks_ready);
        sleep(rand()%5+1);
    }
}
void printer_function(int printer_id){
    while (1){
        sem_wait(tasks_ready);
        sem_wait(queue_occupied);
        for(int i=0;i<MAX_PRINT_TASKS;i++){
            if(tasksQueue->tasks[i].is_occupied){
                printf("Printer %d is printing\n",printer_id);
                for(int j=0;j<TEXT_SIZE;j++){
                    printf("%c",tasksQueue->tasks[i].text[j]);
                    fflush(stdout);
                    sleep(1);
                }
                printf("\n");
                tasksQueue->tasks[i].is_occupied = 0;
                break;
            }
        }
        sem_post(queue_occupied);
        sem_post(tasks_available);
    }
}