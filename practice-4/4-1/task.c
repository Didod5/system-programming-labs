#include <stdio.h>
#include <stdlib.h>

int main() {
    int initial_size = 10;
    int new_size = 20;

    // Выделение памяти под 10 элементов
    int *arr = (int *)malloc(initial_size * sizeof(int));

    // Проверка успешности выделения памяти
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    // Заполнение массива
    printf("Массив из 10 элементов:\n");
    for (int i = 0; i < initial_size; i++) {
        arr[i] = i + 1;
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Увеличение массива до 20 элементов
    int *temp = (int *)realloc(arr, new_size * sizeof(int));

    // Проверка realloc
    if (temp == NULL) {
        free(arr);
        perror("realloc");
        return 1;
    }

    arr = temp;

    // Заполнение новых элементов
    for (int i = initial_size; i < new_size; i++) {
        arr[i] = i + 1;
    }

    // Вывод увеличенного массива
    printf("Массив после realloc до 20 элементов:\n");
    for (int i = 0; i < new_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Освобождение памяти
    free(arr);

    return 0;
}