# Практика 3-2 — Именованные каналы FIFO

## Описание

Программа использует именованный канал FIFO
для взаимодействия между двумя процессами.

Процесс writer читает содержимое файла и записывает его в FIFO.

Процесс reader получает данные из FIFO
и подсчитывает количество слов.

Используются:

- `mkfifo()`
- `open()`
- `read()`
- `write()`
- `close()`

---

## Компиляция

```bash
gcc -Wall -Wextra -o writer writer.c
gcc -Wall -Wextra -o reader reader.c
```

---

## Запуск

Сначала запустить reader:

```bash
./reader
```

Затем writer:

```bash
./writer text.txt
```
