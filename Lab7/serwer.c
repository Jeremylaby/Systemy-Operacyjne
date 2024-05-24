//
// Created by stani on 09.05.2024.
//
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SERVER_QUEUE "/server_queue"
#define MAX_CLIENTS 5
#define INIT "INIT"
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)
typedef struct{
    mqd_t queue;
    int id;
}client_t;
client_t clients[10];
int clients_number=0;
void handle_client_msg(const char*msg,const char*client_name);
int main() {
    mqd_t server_queue;
    mq_unlink(SERVER_QUEUE);
    struct mq_attr attr;
    char buffer[MSG_BUFFER_SIZE];
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    server_queue = mq_open(SERVER_QUEUE, O_CREAT | O_RDWR, 0644, &attr);
    printf("server listening...\n");
    while(1){
        ssize_t bytes_read= mq_receive(server_queue,buffer,MSG_BUFFER_SIZE,NULL);
        if(bytes_read>0){
            buffer[bytes_read]='\0';
            char client_name[20];
            sscanf(buffer,"%s",client_name);
            handle_client_msg(buffer+ strlen(client_name)+1,client_name);
        }
    }
    mq_close(server_queue);
    mq_unlink(SERVER_QUEUE);
    return 0;
}
void handle_client_msg(const char*msg,const char*client_name){
    if(strcmp(INIT,msg)==0){
        if(clients_number<MAX_CLIENTS){
            mqd_t client_queue = mq_open(client_name,O_WRONLY);
            clients[clients_number].queue=client_queue;
            clients[clients_number].id=clients_number;
            char client_msg[30];
            sprintf(client_msg,"Your ID is:%d",clients_number);
            mq_send(client_queue,client_msg, strlen(client_msg)+1,0);
            clients_number++;
            printf("New client created\n");
        }else{
            perror("Max number of clients\n");
        }
    }else{
        int curr_id;
        curr_id= atoi(client_name);
        char message[MAX_MSG_SIZE];
        sprintf(message,"%s %s",client_name,msg);
        printf("Message from client with ID: %s message text: %s\n",client_name,msg);
        for(int i=0;i<clients_number;i++){
            if(i!=curr_id){
                mq_send(clients[i].queue,message,strlen(message)+1,0);
            }
        }
    }
}