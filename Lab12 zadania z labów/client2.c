// Client side implementation of UDP client-server model
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#define BACKLOG 5
#define BUF_SIZE 1024
#define MAX_MSG_SIZE 950
#define MAX_NAME_LENGTH 20
int sfd;
struct sockaddr_in	 servaddr;
char name[MAX_NAME_LENGTH];
void *receive_message() {
    while (1) {
        char message[BUF_SIZE];
        socklen_t len;
        int n;
        n = recvfrom(sfd, (char *)message, BUF_SIZE,
                     MSG_WAITFORONE, (struct sockaddr *) &servaddr,
                     &len);
        if (n > 0) {
            printf("%s\n", message);
            fflush(stdout);
        }
    }
    return NULL;
}

void handle_sigint() {
    char message_to_send[BUF_SIZE];
    sprintf(message_to_send,"%s %s",name,"STOP");
    sendto(sfd, (const char *)message_to_send, strlen(message_to_send),
           MSG_WAITFORONE, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));
    close(sfd);
    printf("Disconnected from server.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <name> <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }
    strcpy(name, argv[1]);
    int port = atoi(argv[2]);

    if ( (sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n;
    socklen_t len;

    char msg[BUF_SIZE];
    sprintf(msg,"%s %s",name,"INIT");
    sendto(sfd, (const char *)msg, strlen(msg),
           MSG_WAITFORONE, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));
    char msg2[BUF_SIZE];
    recvfrom(sfd, (char *) msg2, BUF_SIZE,
             MSG_WAITFORONE , (struct sockaddr *) &servaddr,
             &len);
    msg2[BUF_SIZE]='\0';
    printf("Server : %s\n",msg2);
    if(strcmp("CONNECTED",msg2)!=0){
        return 0;
    }
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;

    sigaction(SIGINT, &sa, NULL);
    pthread_t receipt_thread;
    pthread_create(&receipt_thread, NULL, receive_message, NULL);
    while (1) {
        char message[MAX_MSG_SIZE];
        char message_to_send[BUF_SIZE];
        if (fgets(message, MAX_MSG_SIZE, stdin) != NULL) {
            message[strcspn(message, "\n")] = '\0';
            sprintf(message_to_send,"%s %s",name,message);
            if (strcmp(message, "STOP") == 0) {
                sendto(sfd, (const char *)message_to_send, strlen(message_to_send),
                       MSG_WAITFORONE, (const struct sockaddr *) &servaddr,
                       sizeof(servaddr));
                close(sfd);
                printf("Disconnected from server.\n");
                exit(0);
            };
            if (sendto(sfd, (const char *)message_to_send, strlen(message_to_send),
                       MSG_WAITFORONE, (const struct sockaddr *) &servaddr,
                       sizeof(servaddr)) == -1) {
                perror("Failed to send message");
                break;
            }

            printf("Message sent: %s\n", message);
        }
    }
    close(sfd);
    return 0;
}

