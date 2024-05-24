#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    mqd_t mq;
    struct mq_attr attr;

    // Ustawienia atrybutów kolejki
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;

    // Tworzenie kolejki komunikatów
    mq = mq_open("/test_queue", O_CREAT | O_RDWR, 0644, &attr);
    if (mq == (mqd_t) -1) {
        perror("mq_open");
        exit(1);
    }

    printf("Kolejka komunikatów utworzona.\n");

    // Wysłanie wiadomości
    const char *message = "Hello, WSL!";
    if (mq_send(mq, message, strlen(message) + 1, 0) == -1) {
        perror("mq_send");
        exit(1);
    }
    printf("Wiadomość wysłana.\n");

    // Odbieranie wiadomości
    char buffer[256];
    if (mq_receive(mq, buffer, 256, NULL) == -1) {
        perror("mq_receive");
        exit(1);
    }
    printf("Odebrana wiadomość: %s\n", buffer);

    // Zamknięcie kolejki
    if (mq_close(mq) == -1) {
        perror("mq_close");
        exit(1);
    }

    // Usunięcie kolejki komunikatów
    if (mq_unlink("/test_queue") == -1) {
        perror("mq_unlink");
        exit(1);
    }
    printf("Kolejka komunikatów usunięta.\n");

    return 0;
}
