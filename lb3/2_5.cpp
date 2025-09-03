/*Перехват сигнала ctrl-z многократно для процесса с воссстановлением исходного обработчика.*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iomanip>
#include <cstring>

size_t count = 0;

void handler(int sig){
    ++count;
    std::cout << "\nСигнал SIGTSTP получен!\n";
    if (count > 5){
    	struct sigaction default_action;
    	default_action.sa_handler = SIG_DFL;
    	sigemptyset(&default_action.sa_mask);
    	sigaction(SIGTSTP, &default_action, nullptr);
    }
    
}
int main() {
    std::cout << "Программа начала свою работу.\n";
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTSTP, &sa, nullptr);
    while (true){
    	sleep(1);
    }
}
