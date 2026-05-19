#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main() {
    const char *filename = "mapped_file.txt";
    const char *text = "Hello from mmap!";
    size_t text_size = strlen(text) + 1;

    // Открытие файла
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Установка размера файла
    if (ftruncate(fd, text_size) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    // Получение информации о файле
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    // Отображение файла в память
    char *mapped = mmap(
        NULL,
        sb.st_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );

    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // Запись строки через отображённую область
    strcpy(mapped, text);

    printf("Записано в файл через mmap: %s\n", mapped);

    // Синхронизация с файлом (необязательно, но полезно)
    if (msync(mapped, sb.st_size, MS_SYNC) == -1) {
        perror("msync");
    }

    // Освобождение отображения
    if (munmap(mapped, sb.st_size) == -1) {
        perror("munmap");
    }

    // Закрытие файла
    close(fd);

    return 0;
}