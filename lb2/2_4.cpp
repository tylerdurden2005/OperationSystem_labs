
#include <iostream>
#include <unistd.h>
#include <wait.h>
void situationA(){
    std::cout << "Ситуация a)\n";
    int pid = fork();
    if (pid == -1){
        std::cout << "Ошибка: Дочерний процесс не создался!\n";
        exit(EXIT_FAILURE);
    }
    else if (pid==0){
        std::cout << "Дочерний процесс: pid="<<getpid()<<" ppid="<<getppid() << "\n";
        sleep(2);
        exit(0);
    }
    else{
        std::cout << "Родитель: pid="<<getpid() << "\n";
        wait(nullptr);
        std::cout << "Дочерний процесс завершился: pid="<< pid << "\n";
        std::cout << "Родитель завершился.\n";
    }
}

void situationB() {
    std::cout << "Ситуация б)\n";
    int pid = fork();
    if (pid == -1) {
        std::cout << "Ошибка: Дочерний процесс не создался!\n";
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        std::cout << "Дочерний процесс: pid=" << getpid() << " ppid=" << getppid() << "\n";
        sleep(2);
        std::cout << "Дочерний процесс завершился: pid=" << getpid() << " ppid=" << getppid() << "\n";

    } else {
        std::cout << "Родитель: pid=" << getpid() << "\n";
        std::cout << "Родитель завершился.\n";
    }
    system("ps -l >> father_sonB.txt");
}
void situationC(){
    std::cout << "Ситуация c)\n";
    int pid = fork();
    if (pid == -1) {
        std::cout << "Ошибка: Дочерний процесс не создался!\n";
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        std::cout << "Дочерний процесс: pid=" << getpid() << " ppid=" << getppid() << "\n";
        std::cout << "Дочерний процесс завершился: pid=" << getpid() << " ppid=" << getppid() << "\n";
    } else {
        std::cout << "Родитель: pid=" << getpid() << "\n";
        sleep(15);
        std::cout << "Родитель завершился.\n";
    }
    system("ps -l >> father_sonC.txt");
}
int main() {

    situationA();
    system(">father_sonB.txt");
    int pid = fork();
    if (pid ==0){
        situationB();
        exit(0);
    }
    else{
        wait(nullptr);
        sleep(3);
    }
    system(">father_sonC.txt");
    situationC();
    return 0;
}
