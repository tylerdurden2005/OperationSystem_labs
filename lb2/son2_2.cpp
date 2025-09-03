#include <iostream>
#include <unistd.h>
#include <wait.h>

int main(){
    std::cout << "Дочерний процесс: pid="<<getpid()<<" ppid="<<getppid() << "\n";
    sleep(10);
    exit(1); 
    return 0;  // статус завершения 0
}
