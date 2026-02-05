#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 3

const char *filenames[THREADS] = {
    "file1.txt",
    "file2.txt",
    "file3.txt"
};

const char *contents[THREADS] = {
    "Hello",
    "Multithreading",
    "Linux"
};

typedef struct {
    const char *filename;
    long count;
} thread_data_t;

void *count_chars(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    FILE *f = fopen(data->filename, "r");

    if (!f) {
        perror(data->filename);
        pthread_exit(NULL);
    }

    data->count = 0;
    while (fgetc(f) != EOF) {
        data->count++;
    }

    fclose(f);
    pthread_exit(NULL);
}

int main(void) {
    /* ---------- Create files ---------- */
    for (int i = 0; i < THREADS; i++) {
        FILE *f = fopen(filenames[i], "w");
        if (!f) {
            perror(filenames[i]);
            return 1;
        }
        fputs(contents[i], f);
        fclose(f);
    }

    pthread_t threads[THREADS];
    thread_data_t data[THREADS];
    long total = 0;

    /* ---------- Create threads ---------- */
    for (int i = 0; i < THREADS; i++) {
        data[i].filename = filenames[i];
        data[i].count = 0;

        if (pthread_create(&threads[i], NULL, count_chars, &data[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    /* ---------- Join threads ---------- */
    for (int i = 0; i < THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
        total += data[i].count;
    }

    printf("Total characters in all files: %ld\n", total);
    return 0;
}
