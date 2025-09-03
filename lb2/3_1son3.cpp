#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <signal.h>


void handler(int sig) {
    signal(SIGUSR1, SIG_DFL);
    std::cout << "Сигнал обработан по умолчанию!\n";
}

int main() {
    signal(SIGUSR1, handler); 
    std::cout << "son3 pid="<<getpid()<<" перехватывает сигнал:  ";
    sleep(3);
    return 0;
}
