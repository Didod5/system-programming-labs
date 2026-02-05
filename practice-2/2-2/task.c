#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 3
#define OPERATIONS 100

int balance = 1000;
pthread_mutex_t mutex;

void *add_money(void *arg) {
    for (int i = 0; i < OPERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        balance += 10;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(void) {
    pthread_t threads[THREADS];

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        return 1;
    }

    for (int i = 0; i < THREADS; i++) {
        if (pthread_create(&threads[i], NULL, add_money, NULL) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    pthread_mutex_destroy(&mutex);

    printf("Final balance: %d\n", balance);
    return 0;
}
