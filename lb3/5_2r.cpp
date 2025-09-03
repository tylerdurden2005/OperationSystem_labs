/*Программа читает сообщение другого процесса, используя мьютекс и разделение памяти */

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
#define BUF_SIZE 256

struct sharedMemory {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    char buffer[BUF_SIZE];
    bool flag_ready;
};

int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    sharedMemory* shm_ptr = (sharedMemory*)mmap(0, sizeof(sharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    while (1) {
        pthread_mutex_lock(&shm_ptr->mutex);
        while (!shm_ptr->flag_ready) {
            pthread_cond_wait(&shm_ptr->cond, &shm_ptr->mutex); // ждем пока писатель запишет
        }

        std::cout << "Прочитано: " << shm_ptr->buffer << std::endl;
        shm_ptr->flag_ready = false;
        pthread_cond_signal(&shm_ptr->cond); // сигнал писателю, что можно писать
        pthread_mutex_unlock(&shm_ptr->mutex);
        if (strncmp(shm_ptr->buffer, "exit", 4) == 0) break;
    }

    munmap(shm_ptr, sizeof(sharedMemory));
    close(shm_fd);
    return 0;
}
