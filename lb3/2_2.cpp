/*Перехват сигнала ctrl-c однократно для потока с воссстановлением исходного обработчика.*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iomanip>
#include <sys/syscall.h>
#include <cstring>


void handler(int sig){
    pid_t tid = syscall(SYS_gettid);
    std::cout << "\nСигнал SIGINT получен потоком: " << tid << std::endl;
    
    struct sigaction default_action;
    default_action.sa_handler = SIG_DFL;
    sigemptyset(&default_action.sa_mask);
    sigaction(SIGINT, &default_action, nullptr);
}
void* threadFunction(void* arg) {
    sigset_t unblock_set;
    sigemptyset(&unblock_set);
    sigaddset(&unblock_set, SIGINT);
    pthread_sigmask(SIG_UNBLOCK, &unblock_set, nullptr);
    
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);

    while (true){
    	sleep(1);
    }
    return nullptr;
}
int main() {
    pid_t tid = syscall(SYS_gettid);
    std::cout << "Программа начала свою работу. Tid главного процесса: " << tid << std::endl;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_BLOCK, &set, nullptr);
    
    pthread_t t;
    pthread_create(&t, nullptr, threadFunction, nullptr);

    pthread_join(t, nullptr);

}
