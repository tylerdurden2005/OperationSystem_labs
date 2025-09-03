#include <iostream>
#include <unistd.h>
#include <wait.h>

int main()
{
    int status;
    std::cout << "Родитель: pid="<<getpid()<<" ppid="<<getppid() << "\n";
    if(fork()==0)
        execl("./son2_2", "son2_2", NULL);
    system("ps -xf > father_son.txt");
    system("ps -l > father_son2.txt");
    wait(&status);
    std::cout << "Дочерний процесс завершился со статусом: " << status << "\n";
    return 0;
}
