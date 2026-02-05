#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* ---------- Child process ---------- */
        printf("Child process started\n");
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());

        long long sum = 0;
        for (int i = 1; i <= 1000; i++) {
            sum += i;
        }

        printf("Child: sum from 1 to 1000 = %lld\n", sum);

        /* exec used just to demonstrate usage */
        execlp("echo", "echo", "Child process finished", NULL);

        /* If exec fails */
        perror("exec");
        exit(1);
    } else {
        /* ---------- Parent process ---------- */
        int status;

        if (wait(&status) < 0) {
            perror("wait");
            return 1;
        }

        printf("Parent process continued\n");
        printf("Parent PID: %d, Child PID: %d\n", getpid(), pid);

        long long product = 1;
        for (int i = 1; i <= 10; i++) {
            product *= i;
        }

        printf("Parent: product from 1 to 10 = %lld\n", product);
    }

    return 0;
}
