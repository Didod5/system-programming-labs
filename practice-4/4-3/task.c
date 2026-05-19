#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

volatile sig_atomic_t running = 1;

// Обработчик сигнала завершения
void handle_signal(int sig) {
    (void)sig;
    running = 0;
}

// Функция записи в лог
void write_log(const char *message) {
    FILE *log = fopen(
    "/mnt/d/Education/8 семестр/Системное программирование/system-programming-labs/daemon.log",
    "a"
);

    if (log == NULL) {
        return;
    }

    time_t now = time(NULL);
    char *time_str = ctime(&now);

    // Удаляем \n у времени
    time_str[strlen(time_str) - 1] = '\0';

    fprintf(log, "[%s] %s\n", time_str, message);

    fclose(log);
}

int main() {
    pid_t pid;

    // Создание дочернего процесса
    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    // Завершаем родительский процесс
    if (pid > 0) {
        printf("Daemon started with PID %d\n", pid);
        return 0;
    }

    // Создание новой сессии
    if (setsid() < 0) {
        perror("setsid");
        return 1;
    }

    // Сброс маски прав
    umask(0);

    // Переход в корневую директорию
    if (chdir("/") < 0) {
        perror("chdir");
        return 1;
    }

    // Закрытие стандартных потоков
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Обработка сигналов
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    write_log("Daemon started");

    // Основной цикл демона
    while (running) {

        if (access("/mnt/d/Education/8 семестр/Системное программирование/system-programming-labs/config.txt", F_OK) == 0) {
            write_log("config.txt found");
        } else {
            write_log("config.txt NOT found");
        }

        sleep(10);
    }

    write_log("Daemon stopped");

    return 0;
}