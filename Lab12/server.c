#include <stdarg.h>
//#include "lib/error_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define MAX_NAME_LENGTH 20
#define BACKLOG 5
#define BUF_SIZE 1024
#define MAX_MSG_SIZE 950
typedef struct {
    char name[MAX_NAME_LENGTH];
    struct sockaddr_in addr;
} client_t;
client_t clients[BACKLOG];
int clients_num = 0;
int sfd;
struct sockaddr_in servaddr;
int find_idx(char *name);
void add_client(char *name, struct sockaddr_in cli_addr);

void remove_client(int idx);

void list_clients( int idx);

void send_message_to_all(int idx, char *message);

void send_message_to(int sender, int receiver, char *message);

void process_client_command(int idx);

int init_server(int port);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Port is missing\n");
        exit(1);
    }
    int port = atoi(argv[1]);
    init_server(port);


    for (;;) {
        char buff[BUF_SIZE];
        struct sockaddr_in cli_addr;
        socklen_t len = sizeof(cli_addr);
        recvfrom(sfd, (char *) buff, BUF_SIZE,
                           MSG_WAITALL, (struct sockaddr *) &cli_addr,
                           &len);
        char name[MAX_NAME_LENGTH];
        char command[30];
        sscanf(buff, "%s %s", name, command);
        if (strcmp("INIT", command) == 0) {
            add_client(name,cli_addr);
        }else if(strcmp("STOP",command)==0){
            int idx = find_idx(name);
            remove_client(idx);
        }else if(strcmp("LIST",command)==0){
            int idx = find_idx(name);
            list_clients(idx);
        }else if(strcmp("2ONE",command)==0){
            char name_to[MAX_NAME_LENGTH];
            char msg_to_send[MAX_MSG_SIZE];
            msg_to_send[MAX_MSG_SIZE-1]='\0';
            sscanf(buff,"%s %s %s %s",name,command,name_to,msg_to_send);
            int sender = find_idx(name);
            int receiver = find_idx(name_to);
            send_message_to(sender,receiver,msg_to_send);
        }
        else if(strcmp("2ALL",command)==0){
            char msg_to_send[MAX_MSG_SIZE];
            sscanf(buff,"%s %s %s",name,command,msg_to_send);
            int sender = find_idx(name);
            send_message_to_all(sender,msg_to_send);
        }

    }
    close(sfd);
    return 0;
}

int init_server(int port) {
    close(port);
    if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sfd, (const struct sockaddr *) &servaddr,
             sizeof(servaddr)) < 0) {
        perror("bind failed\n");
        exit(EXIT_FAILURE);
    }
}

void add_client(char *name, struct sockaddr_in cli_addr) {

    if(find_idx(name)!=-1){
        char message[BUF_SIZE] = "ALREADY IN DATABASE";
        printf("User already in database\n");
        sendto(sfd, &message, BUF_SIZE, MSG_WAITFORONE, (const struct sockaddr *) &cli_addr, sizeof(cli_addr));
        return;
    }
    char buff[BUF_SIZE];
    if (clients_num == BACKLOG) {
        char message[BUF_SIZE] = "FAILED";
        sendto(sfd, &message, BUF_SIZE, MSG_WAITFORONE, (const struct sockaddr *) &cli_addr, sizeof(cli_addr));

    } else {
        char message[BUF_SIZE] = "CONNECTED";
        strncpy(clients[clients_num].name, name, MAX_NAME_LENGTH);
        clients[clients_num].addr = cli_addr;
        printf("Added new client: %s \n", clients[clients_num].name);
        sendto(sfd, &message, BUF_SIZE, MSG_WAITFORONE, (const struct sockaddr *) &cli_addr, sizeof(cli_addr));
        clients_num++;
    }
}

int find_idx(char *name){
    for (int i = 0; i < clients_num; i++) {
        if (strcmp(name, clients[i].name) == 0) {
            return i;
        }
    }
    return -1;
}
void remove_client(int idx) {
    if(idx==-1){
        printf("Wrong user\n");
        return;
    }
    printf("Client %s disconnected\n", clients[idx].name);

    for (int i = idx + 1; i < clients_num; i++) {
        clients[i] = clients[i - 1];
    }
    clients_num--;
}

void list_clients( int idx) {
    if(idx==-1){
        printf("Wrong user\n");
        return;
    }
    char list[BUF_SIZE] = "";
    for (int i = 0; i < clients_num; i++) {
        if (i == idx) {
            strcat(list, "(you): ");
        } else { strcat(list, "(user): "); }
        strcat(list, clients[i].name);
        strcat(list, "\n");
    }
    sendto(sfd, &list, BUF_SIZE, MSG_WAITFORONE, (const struct sockaddr *) &clients[idx].addr, sizeof(clients[idx].addr));
    printf("List was sent\n");
}

void send_message_to_all(int idx, char *message) {
    if(idx==-1){
        printf("Wrong user\n");
        return;
    }
    for (int i = 0; i < clients_num; i++) {
        char final_message[BUF_SIZE];
        snprintf(final_message, sizeof(final_message), "[ %s ]: %s ", clients[idx].name, message);
        if (i != idx) {
            sendto(sfd, &final_message, BUF_SIZE, MSG_WAITFORONE, (const struct sockaddr *) &clients[i].addr, sizeof(clients[i].addr));
        }
    }
    printf("Message was sent to all clients\n");
}

void send_message_to(int sender, int receiver, char *message) {

    if (receiver == -1||sender==-1) {
        printf("Wrong name\n");
        return;
    }
    char final_message[BUF_SIZE];
    time_t current_time;
    struct tm *time_info;
    char time_string[64];
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%d-%m-%Y %H:%M:%S", time_info);
    snprintf(final_message, sizeof(final_message), "[ %s ]: %s %s", clients[sender].name, message, time_string);
    sendto(sfd, &final_message, BUF_SIZE, MSG_WAITFORONE, (const struct sockaddr *) &clients[receiver].addr, sizeof(clients[receiver].addr));

    printf("Message was sent to %s\n", clients[receiver].name);
}



