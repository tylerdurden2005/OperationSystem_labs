/*Программа пишет сообщение другому процессу, используя мьютекс и разделение памяти */

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
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); // объект разделяемой памяти 
    ftruncate(shm_fd, sizeof(sharedMemory)); // выделяем под него память
    sharedMemory* shm_ptr = (sharedMemory*)mmap(0, sizeof(sharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0); // отображаем этот объект памяти в адресное пространство процесса

    pthread_mutexattr_t mutex_attr; // атрибут взаимного исключения
    pthread_condattr_t cond_attr; // атрибут условной переменной

    pthread_mutexattr_init(&mutex_attr); // инициализация структуры
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED); // установка значения атрибута совместного использования для заданного мьютекса

    pthread_condattr_init(&cond_attr); // инициализация структуры
    pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED); // установка значения атрибута совместного использования для заданного мьютекса

    pthread_mutex_init(&shm_ptr->mutex, &mutex_attr); // инициализирует само исключение по атрибутам из mutex_attr
    pthread_cond_init(&shm_ptr->cond, &cond_attr); 
   
    shm_ptr->flag_ready = false; 
    char input[BUF_SIZE];
    while (1) {
        pthread_mutex_lock(&shm_ptr->mutex); // переводит мьютекс из разблокированного на заблокированное
        while (shm_ptr->flag_ready) { // пока предыдущее сообщение еще не прочитано 
            pthread_cond_wait(&shm_ptr->cond, &shm_ptr->mutex); // ждем пока читатель прочтет (автоматически отпускается мьютекс)
        }

        std::cout << "Введите сообщение: ";
        fgets(input, BUF_SIZE, stdin);
        strncpy(shm_ptr->buffer, input, BUF_SIZE);
        shm_ptr->flag_ready = true;

        pthread_cond_signal(&shm_ptr->cond); // отправляет сигнал, с помощью условной переменной, читателю, что можно читать
        pthread_mutex_unlock(&shm_ptr->mutex); //переводит мьютекс из заблокированного на разблокированное

        if (strncmp(input, "exit", 4) == 0) break;
    }

    munmap(shm_ptr, sizeof(sharedMemory));
    shm_unlink(SHM_NAME);
    close(shm_fd);
    return 0;
}
