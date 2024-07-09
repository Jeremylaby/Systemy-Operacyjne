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
#define MAXLINE 1024
/*
 * Napisz server Datagramowy który Domeny internetowej
 * dostępny na porcie 3000
 * którego zadaniem jest odebranie pojedyńczej wiadomości
 * i wypisanie jej na ekran*/
// Driver code
struct sockaddr_in	 servaddr,cliaddr;
int main() {
    int sockfd;
    char buffer[MAXLINE];
    const char *hello = "Hello from server";

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
              sizeof(servaddr)) < 0 )
    {
        perror("bind failed\n");
        exit(EXIT_FAILURE);
    }
    socklen_t len;
    int n;

    len = sizeof(cliaddr);  //len is value/result

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                 &len);
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);
    sendto(sockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
           len);
    close(sockfd);
    return 0;
}


