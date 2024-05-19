//
// Created by stani on 09.05.2024.
//
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define QUEUE_PREFIX "/client_queue"
#define SERVER_QUEUE "/server_queue"
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)
#define INIT "INIT"
void send_msg(mqd_t server_queue,const char* client_name,const char* msg);
int main() {
    mqd_t server_queue, client_queue;
    struct mq_attr attr;
    char buffer[MSG_BUFFER_SIZE];
    char client_id[20];

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    char queue_name[40];
    sprintf(queue_name,"%s%d",QUEUE_PREFIX,getpid());

    mq_unlink(queue_name);
    server_queue= mq_open(SERVER_QUEUE,O_WRONLY);
    client_queue = mq_open(queue_name, O_CREAT | O_RDONLY, 0644, &attr);
    send_msg(server_queue,queue_name,INIT);
    ssize_t bytes_read = mq_receive(client_queue,buffer,MAX_MSG_SIZE,NULL);
    if(bytes_read>=0){
        buffer[bytes_read] = '\0';
        sscanf(buffer, "Your ID is:%s", client_id);
        printf("%s\n", buffer);
    }else {
        perror("mq_receive");
        return -1;
    }

    if(fork()==0){
        char buffer2[MSG_BUFFER_SIZE];
        while(1){
            ssize_t bytes_read = mq_receive(client_queue,buffer2,MSG_BUFFER_SIZE,NULL);
            if(bytes_read>=0){
                buffer[bytes_read] = '\0';
                printf("Msg received: %s\n",buffer2);
            }
        }
    }

    char input[MAX_MSG_SIZE];
    while (fgets(input, MAX_MSG_SIZE, stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        sprintf(buffer, "%s", input);
        send_msg(server_queue, client_id, buffer);
    }

    wait(NULL);
    mq_close(client_queue);
    mq_unlink(queue_name);
    mq_close(server_queue);
    return 0;


}
void send_msg(mqd_t server_queue,const char* client_name,const char* msg){
    char buffer[MSG_BUFFER_SIZE];
    sprintf(buffer,"%s %s",client_name,msg);
    mq_send(server_queue,buffer,strlen(buffer)+1,0);
    printf("message send\n");
}