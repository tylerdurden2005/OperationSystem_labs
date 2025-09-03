#!/bin/bash

dir="dir_for_test"
mkdir "$dir"
echo "Размер каталога после создания: "$(ls -ld "$dir" | awk '{print $5}')" байт"
echo "Размер содержимого каталога после создания: "$(du -sh "$dir" | awk '{print $1}')
echo "Добавляем 500 папок"
for i in {1..500};
do
mkdir "$dir/dir$i"
done
echo "Размер каталога: "$(ls -ld "$dir" | awk '{print $5}')" байт"
echo "Размер содержимого каталога: "$(du -sh "$dir" | awk '{print $1}')
echo "Добавляем 500 файлов"
for i in {1..500};
do
touch "$dir/file$i.txt"
done
echo "Размер каталога: "$(ls -ld "$dir" | awk '{print $5}')" байт"
echo "Размер содержимого каталога: "$(du -sh "$dir" | awk '{print $1}')
echo "Добавляем большой файл(20 МБ)"
dd if=/dev/zero of="$dir/bigfile" bs=1M count=20 status=none
echo "Размер каталога: "$(ls -ld "$dir" | awk '{print $5}')" байт"
echo "Размер содержимого каталога: "$(du -sh "$dir" | awk '{print $1}')
echo "Удаляем все папки"
rmdir "$dir"/dir*
echo "Размер каталога: "$(ls -ld "$dir" | awk '{print $5}')" байт"
echo "Размер содержимого каталога: "$(du -sh "$dir" | awk '{print $1}')
#ls "$dir"
echo "Удаляем все файлы"
rm "$dir"/file*.txt "$dir"/bigfile
echo "Размер каталога: "$(ls -ld "$dir" | awk '{print $5}')" байт"
echo "Размер содержимого каталога: "$(du -sh "$dir" | awk '{print $1}')
rm -rf "$dir"
echo "Каталог удалён"
