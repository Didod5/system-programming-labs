#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 1024

int main() {
    int fifo_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    int words = 0;
    int inWord = 0;

    // Открытие FIFO на чтение
    fifo_fd = open(FIFO_NAME, O_RDONLY);
    if (fifo_fd == -1) {
        perror("open");
        return 1;
    }

    // Чтение данных из FIFO
    while ((bytesRead = read(fifo_fd, buffer, BUFFER_SIZE)) > 0) {

        for (ssize_t i = 0; i < bytesRead; i++) {

            if (isspace((unsigned char)buffer[i])) {
                inWord = 0;
            } else if (!inWord) {
                inWord = 1;
                words++;
            }
        }
    }

    if (bytesRead == -1) {
        perror("read");
        close(fifo_fd);
        return 1;
    }

    printf("Word count: %d\n", words);

    close(fifo_fd);

    // Удаление FIFO
    if (unlink(FIFO_NAME) == -1) {
        perror("unlink");
    }

    return 0;
}