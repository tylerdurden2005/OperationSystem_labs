/*программа предназначена для очистки системных ресурсов (семафоров и разделяемой памяти)*/

#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>

#define SHM_NAME "/my_shm"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"

int main() {
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    shm_unlink(SHM_NAME);
    std::cout << "Ресурсы очищены." << std::endl;
    return 0;
}
