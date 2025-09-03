/*Программа создает 2 потока, первый поток отправляет второму сигнал для его удаления. Используется собственный обработчик сигнала*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iomanip>
#include <sys/time.h>
#include <chrono>


void handle(int sig){
    if (sig == SIGUSR1){
        std::cout << "Нить №2 получила сигнал SIGUSR1\n";
        std::cout << "Нить №2 завершается...\n";
        pthread_exit(nullptr);
    }
}
void* threadFunction1(void* arg) {
    pthread_t t2 = *(pthread_t*)arg;
    std::cout << "Нить №1 работает и ждет 3 секунды....\n";
    auto start = std::chrono::high_resolution_clock::now();
    sleep(3);
    
    pthread_kill(t2, SIGUSR1);
    std::cout << "Нить №1 отправила сигнал....\n";
    pthread_join(t2, nullptr);
    auto end = std::chrono::high_resolution_clock::now();
    double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
 	
    std::cout << "Время : " << std::fixed << time_taken << std::setprecision(9) << " секунд\n";
    std::cout << "Нить №1 завершается....\n";
    return nullptr;
}
void* threadFunction2(void* arg) {
    sleep(1);
    struct sigaction sa;
    sa.sa_handler = handle;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, nullptr);
    while (true) {
        while(true){
   	    std::cout << "Нить №2 работает...\n";
   	    sleep(1);
   	}
        pause();
    }
    return nullptr;
}
int main() {
    std::cout << "Программа начала свою работу.\n";
    pthread_t t1, t2;
    pthread_create(&t2, nullptr, threadFunction2, nullptr);
    pthread_create(&t1, nullptr, threadFunction1, &t2);

    pthread_join(t1, nullptr);
    std::cout << "Главный процесс завершился!\n";
}

