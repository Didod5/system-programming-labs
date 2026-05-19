#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int fifo_fd;
    FILE *file;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <text_file>\n", argv[0]);
        return 1;
    }

    // Создание FIFO
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
    }

    // Открытие файла
    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    // Открытие FIFO на запись
    fifo_fd = open(FIFO_NAME, O_WRONLY);
    if (fifo_fd == -1) {
        perror("open");
        fclose(file);
        return 1;
    }

    // Чтение файла и отправка в FIFO
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (write(fifo_fd, buffer, bytesRead) == -1) {
            perror("write");
            close(fifo_fd);
            fclose(file);
            return 1;
        }
    }

    close(fifo_fd);
    fclose(file);

    return 0;
}