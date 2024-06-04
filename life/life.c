#include <pthread.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grid.h"


typedef struct {
    int start_row;
    int end_row;
    char *src;
    char *dst;
} thread_data_t;

void *update_grid_section(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    for (int i = data->start_row; i < data->end_row; ++i) {
        for (int j = 0; j < grid_width; ++j) {
            data->dst[i * grid_width + j] = is_alive(i, j, data->src);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Number of threads must be positive\n");
        return 1;
    }
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    int rows_per_thread = grid_height / num_threads;
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i + 1) * rows_per_thread;
        if (i == num_threads - 1) {
            thread_data[i].end_row = grid_height;
        }
    }

    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr();

    char *foreground = create_grid();
    char *background = create_grid();
    char *tmp;

    init_grid(foreground);

    while (true) {
        draw_grid(foreground);
        usleep(500 * 1000);

        for (int i = 0; i < num_threads; i++) {
            thread_data[i].src = foreground;
            thread_data[i].dst = background;
            pthread_create(&threads[i], NULL, update_grid_section, &thread_data[i]);
        }

        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        tmp = foreground;
        foreground = background;
        background = tmp;
    }

    endwin();
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}
