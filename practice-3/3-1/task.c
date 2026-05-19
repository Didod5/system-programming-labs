#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFER_SIZE 256

int main() {
    int fd[2];
    pid_t pid;

    // Создание канала
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создание дочернего процесса
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // ----- Дочерний процесс -----

        close(fd[0]); // Закрываем чтение

        int file = open("input.txt", O_RDONLY);

        if (file < 0) {
            perror("open");
            close(fd[1]);
            exit(EXIT_FAILURE);
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        // Чтение файла и передача в канал
        while ((bytes_read = read(file, buffer, BUFFER_SIZE)) > 0) {
            if (write(fd[1], buffer, bytes_read) != bytes_read) {
                perror("write");
                close(file);
                close(fd[1]);
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read < 0) {
            perror("read");
        }

        close(file);
        close(fd[1]);

        exit(EXIT_SUCCESS);

    } else {
        // ----- Родительский процесс -----

        close(fd[1]); // Закрываем запись

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        int lines = 0;
        int has_data = 0;
        char last_char = '\0';
        
        // Чтение из канала и подсчёт строк
        while ((bytes_read = read(fd[0], buffer, BUFFER_SIZE)) > 0) {

            has_data = 1;

            for (ssize_t i = 0; i < bytes_read; i++) {
                if (buffer[i] == '\n') {
                    lines++;
                }
            }
        }

        if (bytes_read < 0) {
            perror("read");
        }

        if (has_data && last_char != '\n') {
            lines++;
        }
        close(fd[0]);

        wait(NULL);

        printf("Количество строк: %d\n", lines);
    }

    return 0;
}