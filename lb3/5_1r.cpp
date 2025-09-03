/*Программа читает сообщение другого процесса, используя семафоры и разделение памяти */

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
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    char* shm_ptr = (char*)mmap(0, BUF_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    sem_t* sem_empty = sem_open(SEM_EMPTY, 0);
    sem_t* sem_full  = sem_open(SEM_FULL,  0);
    while (1) {
        sem_wait(sem_full);
        std::cout << "Прочитано: " << shm_ptr << std::endl;
        sem_post(sem_empty);

        if (strncmp(shm_ptr, "exit", 4) == 0) break;
    }

    munmap(shm_ptr, BUF_SIZE); 
    close(shm_fd);
    return 0;
}
