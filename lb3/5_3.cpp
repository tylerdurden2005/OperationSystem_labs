/*Программа создает потоки, где он читатель, а другой писатель (используется мьютекс и разделение памяти)*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iomanip>
#include <cstring>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>

#define BUF_SIZE 256

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex;

struct sharedMemory {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    char buffer[BUF_SIZE];
    bool flag_ready;
};

sharedMemory shm;

void* threadFunction1(void* arg) 
{
    char input[BUF_SIZE];
    shm.flag_ready = false;
    while (1) {
        pthread_mutex_lock(&shm.mutex); // переводит мьютекс из разблокированного на заблокированное
        while (shm.flag_ready) { // пока предыдущее сообщение еще не прочитано 
            pthread_cond_wait(&shm.cond, &shm.mutex); // ждем пока читатель прочтет (автоматически отпускается мьютекс)
        }
        std::cout << "Введите сообщение: ";
        fgets(input, BUF_SIZE, stdin);
        strncpy(shm.buffer, input, BUF_SIZE);
        shm.flag_ready = true;

        pthread_cond_signal(&shm.cond); // отправляет сигнал, с помощью условной переменной, читателю, что можно читать
        pthread_mutex_unlock(&shm.mutex); //переводит мьютекс из заблокированного на разблокированное

        if (strncmp(input, "exit", 4) == 0) break;
    }
    return nullptr;
}

void* threadFunction2(void* arg) 
{
    while (1) {
        pthread_mutex_lock(&shm.mutex);
        while (!shm.flag_ready) {
            pthread_cond_wait(&shm.cond, &shm.mutex); // ждем пока писатель запишет
        }

        std::cout << "Прочитано: " << shm.buffer << std::endl;
        shm.flag_ready = false;
        pthread_cond_signal(&shm.cond); // сигнал писателю, что можно писать
        pthread_mutex_unlock(&shm.mutex);
        if (strncmp(shm.buffer, "exit", 4) == 0) break;
    }
    return nullptr;
}

int main(void) 
{
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, nullptr);
 
    pthread_create(&thread1, nullptr, threadFunction1, nullptr); // writer
    sleep(2);   // чтобы 1 гарантировано начал выполняться первым
    pthread_create(&thread2, nullptr, threadFunction2, nullptr); // reader
 
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
 
    pthread_mutex_destroy(&mutex);
    return 0;
}
