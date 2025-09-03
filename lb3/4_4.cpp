/*Проверка очереди сигналов (FIFO) для сигналов реального времени*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iomanip>
#include <cstring>

void handler(int sig, siginfo_t *info, void *context) {
    std::cout << "Получен сигнал " << sig << " (значение: " << info->si_value.sival_int << ")" << std::endl;
}

int child(void) {

    sigset_t all_blocked;
    sigfillset(&all_blocked);
    sigprocmask(SIG_SETMASK, &all_blocked, nullptr);

    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    sigfillset(&action.sa_mask); 
    action.sa_sigaction = handler;
 
    sigaction(SIGRTMIN,   &action, nullptr);
    
    sigset_t unblock_set;
    sigemptyset(&unblock_set);
    sigaddset(&unblock_set, SIGRTMIN);   
    sigprocmask(SIG_UNBLOCK, &unblock_set, nullptr);
    while(true) pause();
    return 0;
}

int main() {
    std::cout << "Программа начала работу.\n";
    pid_t cpid = fork();
    
    if (cpid == 0) {
        child();
    } else {
        sleep(2); 
        union sigval value;
        for (int i=0; i<10; ++i){
            value.sival_int = i;
            sigqueue(cpid, SIGRTMIN, value);
        }
        sleep(5); 
    }
    
    std::cout << "Программа завершила работу.\n";
    return 0;
}
