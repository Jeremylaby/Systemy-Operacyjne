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
typedef struct pollfd pollfd_t;
typedef struct {
    int empty;
    int cfd;
    char name[MAX_NAME_LENGTH];
} client_t;
client_t clients[BACKLOG];
int clients_num = 0;
pollfd_t poll_fds[BACKLOG];
int sfd;

void *add_client();

void remove_client(int idx);


void list_clients(int cfd, int idx);

void send_message_to_all(int idx, char *message);

void send_message_to(char *name, char *message, char *sender);

void process_client_command(int idx) {
    int is_waiting = poll_fds[idx].revents & POLLIN;
    if (is_waiting) {
        char buf[BUF_SIZE];
        int numRead;
        numRead = recv(clients[idx].cfd, buf, BUF_SIZE, 0);
        if (numRead == -1) {
            exit(-5);
        }
        if (numRead > 0) {
            char *command = strtok(buf, " ");
            char *message;
            if (strcmp("STOP", command) == 0) {
                remove_client(idx);
            } else if (strcmp("LIST", command) == 0) {
                list_clients(clients[idx].cfd,idx);
            } else if (strcmp("2ALL", command) == 0) {
                message = strtok(NULL, " ");
                send_message_to_all(idx, message);
            } else if (strcmp("2ONE", command) == 0) {
                char *name = strtok(NULL, " ");
                message = strtok(NULL, " ");
                send_message_to(name, message, clients[idx].name);
            } else {
                printf("Command %s invalid\n", command);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Port is missing\n");
        exit(1);
    }
    struct sockaddr_in address;
    int port = atoi(argv[1]);
    close(port);
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Server socket fd = %d\n", sfd);
    if (sfd == -1) {
        printf("Failed creating socket\n");
        exit(1);
    }
    for (int i = 0; i < BACKLOG; i++) {
        clients[i].empty = 1;
    }
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(sfd, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) == -1) {
        printf("Failed binding socket\n");
        exit(2);
    }
    if (listen(sfd, BACKLOG) == -1) {
        printf("Failed listening \n");
        exit(3);
    }
    pthread_t socket_thread;
    pthread_create(&socket_thread, NULL, add_client, NULL);
    for (;;) {
        poll(poll_fds, BACKLOG, 0);
        for (int i = 0; i < clients_num; i++) {
            process_client_command(i);
        }
    }
}

void *add_client() {
    while (1) {
        int numRead;
        char buff[MAX_NAME_LENGTH];
        int client_fd = accept(sfd, NULL, NULL);
        if (client_fd < 0) {
            perror("Failed to accept connection");
            exit(1);
        }
        numRead = recv(client_fd, buff, MAX_NAME_LENGTH, 0);
        if (numRead <= 0) {
            perror("Failed to read from client");
            close(client_fd);
            exit(1);
        }
        if (clients_num == BACKLOG) {
            char message[BUF_SIZE] = "Failed to connect";
            message[BUF_SIZE] = '\0';
            send(client_fd, message, strlen(message), 0);
            continue;
        }
        char message[BUF_SIZE] = "Connected";
        send(client_fd, message, strlen(message), 0);
        clients[clients_num].cfd = client_fd;
        strncpy(clients[clients_num].name, buff, MAX_NAME_LENGTH);
        poll_fds[clients_num].fd = client_fd;
        poll_fds[clients_num].events = POLLIN | POLLOUT;
        printf("Added new client: %s (fd = %d)\n", clients[clients_num].name, client_fd);
        clients_num++;
    }
}

void remove_client(int idx) {
    close(clients[idx].cfd);
    printf("Client %s disconnected\n", clients[idx].name);
    for (int i = idx + 1; i < clients_num; i++) {
        clients[i] = clients[i - 1];
    }
    clients_num--;
}

void list_clients(int cfd,int idx) {
    char list[BUF_SIZE] = "";
    for (int i = 0; i < clients_num; i++) {
        if(i==idx){
            strcat(list,"(you): ");
        }else{strcat(list,"(user): ");}
        strcat(list, clients[i].name);
        strcat(list, "\n");
    }
    send(cfd, list, strlen(list), 0);
    printf("List was sent\n");
}

void send_message_to_all(int idx, char *message) {
    for (int i = 0; i < clients_num; i++) {
        char final_message[BUF_SIZE];
        snprintf(final_message, sizeof(final_message), "[ %s ]: %s ", clients[idx].name,message);
        if (i != idx) {
            send(clients[i].cfd, final_message, strlen(final_message), 0);
        }
    }
    printf("Message was sent to all clients\n");
}

void send_message_to(char *name, char *message, char *sender) {
    int idx = -1;
    for (int i = 0; i < clients_num; i++) {
        if (strcmp(name, clients[i].name) == 0) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
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
    snprintf(final_message, sizeof(final_message), "[ %s ]: %s %s", sender, message,time_string);
    send(clients[idx].cfd, final_message, strlen(final_message), 0);
    printf("Message was sent to %s\n", name);
}

