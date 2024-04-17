//
// Created by stani on 16.04.2024.
//
//
// Created by stani on 11.04.2024.
//
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPE "./pipe"

double f(double x) {
    return 4 / (x * x + 1);
}

double calculate_integral(double a, int n, double dx) {
    double integral = 0.0;
    for (int i = 0; i < n; i++) {
        integral += f(a + i * dx) * dx;
    }
    return integral;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Too few arguments");
        return 1;
    }
    double dx = atof(argv[1]);
    int n = atoi(argv[2]);
    int fd[n][2];
    double a = 0.0 ;
    double b =1.1;
    int fdn = open(PIPE, O_RDONLY);
    read(fdn, &a, sizeof(double));
    read(fdn, &b, sizeof(double));
    close(fdn);
    int m = round((b - a) / dx);
    for (int i = 0; i < n; i++) {
        pipe(fd[i]);
        pid_t pid = fork();
        if (pid == 0) {
            close(fd[i][0]);
            int extra = i == 0 ? m % n : 0;
            double y = calculate_integral((double) (a + i) / n, m / n + extra, dx);
            write(fd[i][1], &y, sizeof(double));
            return 0;
        } else {
            close(fd[i][1]);
        }
    }
    while (wait(NULL) > 0);
    double dy = 0.0;
    for (int i = 0; i < n; i++) {
        double tmp = 0.0;
        read(fd[i][0], &tmp, sizeof(double));
        dy += tmp;

    }
    printf("%lf\n",dy);
    fdn = open(PIPE, O_WRONLY);
    write(fdn, &dy, sizeof(double));
    close(fdn);
    return 0;
}