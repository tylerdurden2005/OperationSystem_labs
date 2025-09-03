/*Программа пишет сообщение другому процессу, используя семафоры и разделение памяти */

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iomanip>
#include <cstring>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHM_NAME "/my_shm"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"
#define BUF_SIZE 256

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); // создается файловый дескриптор разделенной памяти
    ftruncate(shm_fd, BUF_SIZE); // выделяется память
    char* shm_ptr = (char*)mmap(0, BUF_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); // доступ к разделяемой памяти для данного процесса

    sem_t* sem_empty = sem_open(SEM_EMPTY, O_CREAT, 0666, 1); // семафоры
    sem_t* sem_full  = sem_open(SEM_FULL,  O_CREAT, 0666, 0); 

    while (1) {
        /*char input[BUF_SIZE];
        std::cout << "Введите сообщение: ";
        fgets(input, BUF_SIZE, stdin);*/

        sem_wait(sem_empty); // блок если <= 0 иначе уменьшает на 1
        char input[BUF_SIZE];
        std::cout << "Введите сообщение: ";
        fgets(input, BUF_SIZE, stdin);
        strncpy(shm_ptr, input, BUF_SIZE);
        sem_post(sem_full); // увеличивает на 1
 
        if (strncmp(shm_ptr, "exit", 4) == 0) break;
    }

    munmap(shm_ptr, BUF_SIZE); //  удаляет отображение файла разделяемой памяти из адресного пространства процесса
    close(shm_fd);
    return 0;
}
