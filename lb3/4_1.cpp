/*Проверка приоритетов сигналов для сигналов  SIGRTMIN- SIGRTMAX*/

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
    sigaction(SIGRTMIN+10, &action, nullptr);   
    sigaction(SIGRTMAX-2, &action, nullptr);    
    sigaction(SIGRTMAX,   &action, nullptr);   
    sigaction(SIGRTMIN+1,   &action, nullptr);  
    
    /*sigset_t unblock_set;
    sigemptyset(&unblock_set);
    sigaddset(&unblock_set, SIGRTMAX);
    sigaddset(&unblock_set, SIGRTMAX-2);
    sigaddset(&unblock_set, SIGRTMIN+10);
    sigaddset(&unblock_set, SIGRTMIN);*/
    sigprocmask(SIG_UNBLOCK, &all_blocked, nullptr);

    while(true) pause();
    return 0;
}

int main() {
    std::cout << "Программа начала работу.\n";
    pid_t cpid = fork();
    
    if (cpid == 0) {
        child();
    } else {
        sleep(1); 
    
        union sigval value;
        
        
        value.sival_int = 80;
        sigqueue(cpid, SIGRTMAX-2, value); 
        
        value.sival_int = 10;
        sigqueue(cpid, SIGRTMIN+10, value);
        
        value.sival_int = 1;
        sigqueue(cpid, SIGRTMIN, value);
        
        value.sival_int = 2;
        sigqueue(cpid, SIGRTMIN+1, value);
        
        value.sival_int = 99;
        sigqueue(cpid, SIGRTMAX, value);  
        sleep(5); 
    }
    
    std::cout << "Программа завершила работу.\n";
    return 0;
}
