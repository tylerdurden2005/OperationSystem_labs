#include <iostream>
#include <unistd.h>
#include <wait.h>
int main(int argc, char* argv[], char* envp[]) {
    std::cout << "Дочерний процесс: pid="<<getpid()<<" ppid="<<getppid() << "\n";
    std::cout << "Аргументы команды:";
    for (int i =0; i<argc;++i){
        std::cout << " " << argv[i];
    }
    std::cout << "\n";
    std::cout << "Переменные окружения:";
    for (int i =0; i<5;++i){
        std::cout << " " << envp[i];
    }
    std::cout << "\n";
    return 0;
}
