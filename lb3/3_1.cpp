/*Управление сигналами при помощи sigaction().*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iomanip>
#include <cstring>

struct sigaction old_term;

void handler_int(int sig){
    std::cout << "\nСигнал SIGINT получен! Идет обработка...\n";
    sleep(10);
    std::cout << "\nСигнал SIGINT обработан!\n";
}

void handler_usr1(int sig){
    std::cout << "\nСигнал SIGUSR1 получен! Идет обработка....\n";
    sleep(20);
    std::cout << "\nСигнал SIGUSR1 обработан!\n";
}

void handler_stp(int sig){
    std::cout << "\nСигнал SIGTSTP получен!\n";
}
void handler_term(int sig){
    std::cout << "\nСигнал SIGTERM получен!\n";
    sigaction(SIGTERM, &old_term, nullptr);
}
int main() {
    std::cout << "Программа начала свою работу. Его pid = " << getpid() << std::endl;
    struct sigaction sa_int, sa_usr1, sa_stp, sa_term;
    
    sa_stp.sa_handler = handler_stp;
    sigemptyset(&sa_stp.sa_mask);
    sa_stp.sa_flags = SA_ONESHOT; // восстанавливает по умолчанию
    sigaction(SIGTSTP, &sa_stp, nullptr);
    
    sa_int.sa_handler = handler_int;
    sigemptyset(&sa_int.sa_mask);
    sigaddset(&sa_int.sa_mask, SIGUSR1); // маскируем
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, nullptr);
    
    sa_usr1.sa_handler = handler_usr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = SA_NODEFER; // Сигнал не блокируется во время его обработки.
    sigaction(SIGUSR1, &sa_usr1, nullptr);
    
    sa_term.sa_handler = handler_term;
    sigemptyset(&sa_term.sa_mask);
    sa_term.sa_flags = 0;
    sigaction(SIGTERM, &sa_term, &old_term);
    
    while (true){
    	sleep(1);
    }
}
