#include <stdio.h>

// Client side implementation of UDP client-server model
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 3000
#define MAXLINE 10024
/*
 * Napisz server Datagramowy który Domeny internetowej
 * dostępny na porcie 3000
 * którego zadaniem jest odebranie pojedyńczej wiadomości
 * i wypisanie jej na ekran*/
// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    const char *hello = "Hello from client";
    struct sockaddr_in	 servaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd,(const struct sockaddr *)&servaddr, sizeof(servaddr)<0)){
        perror("filed binding");
        exit(1);
    }
    int n;
    socklen_t len;

    sendto(sockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));
    printf("Message sent\n");

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *) &servaddr,
                 &len);
    buffer[n] = '\0';
    printf("Server ;message recived: %s",buffer);
    close(sockfd);
    return 0;
}


