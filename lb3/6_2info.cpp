/*выводит информацию о fifo файле*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>

int main() {
    struct stat st;
    if (stat("customFIFO", &st) == 0) {
        std::cout << "Тип файла: ";
        if (S_ISFIFO(st.st_mode))
            std::cout << "FIFO (именованный канал)\n";

        std::cout << "Размер: " << st.st_size << " байт" << std::endl;
        std::cout << "Inode: " << st.st_ino << std::endl;
        std::cout << "Права доступа: 0" << std::oct << (st.st_mode & 0777) << std::dec << std::endl;
    } 
    // unlink(FIFO_NAME);
    return 0;
}
