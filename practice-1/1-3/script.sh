#!/bin/bash

echo "Создание файла data.txt"
echo "Hello, filesystem!" > data.txt

echo "Создание жёсткой ссылки hardlink.txt"
ln data.txt hardlink.txt

echo "Создание символической ссылки symlink.txt"
ln -s data.txt symlink.txt

echo
echo "Список файлов и ссылок ДО удаления data.txt:"
ls -li data.txt hardlink.txt symlink.txt

echo
echo "Удаление data.txt"
rm data.txt

echo
echo "Список файлов и ссылок ПОСЛЕ удаления data.txt:"
ls -li hardlink.txt symlink.txt

echo
echo "Проверка доступа к файлам:"

echo "- hardlink.txt:"
cat hardlink.txt 2>/dev/null || echo "Не удалось открыть hardlink.txt"

echo
echo "- symlink.txt:"
cat symlink.txt 2>/dev/null || echo "Не удалось открыть symlink.txt"
