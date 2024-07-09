#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#define BACKLOG 5
#define BUF_SIZE 1024
#define MAX_NAME_LENGTH 20
int sfd;

void *receive_message() {
    while (1) {
        char message[BUF_SIZE];
        int numRead;
        numRead = recv(sfd, message, BUF_SIZE, 0);
        if (numRead > 0) {
            printf("%s\n", message);
            fflush(stdout);
        }
    }
    return NULL;
}

void handle_sigint() {
    char *buff = "STOP";
    send(sfd, buff, strlen(buff), 0);
    close(sfd);
    printf("Disconnected from server.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <name> <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }
    char name[MAX_NAME_LENGTH];
    strcpy(name, argv[1]);
    int port = atoi(argv[2]);
    struct sockaddr_in serv_addr;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Client socket fd = %d\n", sfd);
    if (sfd == -1) {
        exit(0);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (connect(sfd, (struct sockaddr *) &serv_addr,
                sizeof(struct sockaddr_in)) == -1) {
        printf("Failed to connect\n");
        exit(1);
    }
    if (send(sfd, name, strlen(name), 0) == -1) {
        perror("send failed");
        close(sfd);
        return -1;
    }
    char buff[BUF_SIZE];
    int numRead = recv(sfd, buff, BUF_SIZE, 0);
    if (numRead >= 0) {
        buff[numRead] = '\0';
        printf("%s\n", buff);
        fflush(stdout);
    }
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;

    sigaction(SIGINT, &sa, NULL);
    if (strcmp(buff, "Connected") != 0) {
        return 1;
    }
    pthread_t receipt_thread;
    pthread_create(&receipt_thread, NULL, receive_message, NULL);
    while (1) {
        char message[BUF_SIZE];
        if (fgets(message, BUF_SIZE, stdin) != NULL) {
            message[strcspn(message, "\n")] = '\0';
            if (strcmp(message, "STOP") == 0) {
                send(sfd, message, BUF_SIZE, 0);
                close(sfd);
                printf("Disconnected from server.\n");
                exit(0);
            }

            if (send(sfd, message, BUF_SIZE, 0) == -1) {
                perror("Failed to send message");
                break;
            }

            printf("Message sent: %s\n", message);
        }
    }

    close(sfd);


}