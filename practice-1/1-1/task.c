#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int main() {
    /* ---------- Create and write input.txt ---------- */
    int fd_write = open("input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_write < 0) {
        perror("open input.txt");
        return 1;
    }

    srand(time(NULL));

    for (int i = 0; i < 50; i++) {
        int num = rand() % 100;   // 0–99

        char buffer[20];
        int len = sprintf(buffer, "%d", num);

        if (write(fd_write, buffer, len) < 0) {
            perror("write number");
            close(fd_write);
            return 1;
        }

        if (i < 49) {
            if (write(fd_write, " ", 1) < 0) {
                perror("write space");
                close(fd_write);
                return 1;
            }
        }
    }

    close(fd_write);

    /* ---------- Read input.txt and find maximum ---------- */
    int fd_read = open("input.txt", O_RDONLY);
    if (fd_read < 0) {
        perror("open input.txt");
        return 1;
    }

    char buffer[1024];
    char num_str[20];
    int num_idx = 0;
    int max = INT_MIN;
    ssize_t bytes;

    while ((bytes = read(fd_read, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes; i++) {
            if (buffer[i] >= '0' && buffer[i] <= '9') {
                num_str[num_idx++] = buffer[i];
            } else if (num_idx > 0) {
                num_str[num_idx] = '\0';
                int num = atoi(num_str);
                if (num > max) {
                    max = num;
                }
                num_idx = 0;
            }
        }
    }

    if (num_idx > 0) {
        num_str[num_idx] = '\0';
        int num = atoi(num_str);
        if (num > max) {
            max = num;
        }
    }

    if (bytes < 0) {
        perror("read");
        close(fd_read);
        return 1;
    }

    close(fd_read);

    /* ---------- Write maximum to max.txt ---------- */
    int fd_max = open("max.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_max < 0) {
        perror("open max.txt");
        return 1;
    }

    char max_str[20];
    int len = sprintf(max_str, "%d\n", max);

    if (write(fd_max, max_str, len) < 0) {
        perror("write max");
        close(fd_max);
        return 1;
    }

    close(fd_max);

    printf("Maximum: %d\n", max);
    return 0;
}
