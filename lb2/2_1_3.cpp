#include <iostream>
#include <unistd.h>
#include <wait.h>
int main() {
    int pid;
    pid = fork();
    int k = 1000;
    int s = 0;
    if (pid == -1){
        std::cout << "Error!!!\n";
        return 1;
    }
    for (int r=0; r<10; ++r){
        if (pid == 0){
            std::cout << "Дочерний процесс: pid=" << getpid() << " ppid=" << getppid()<<std::endl;
            for (int i=0; i<100000000; ++i){}
            for (int j=0; j<4; ++j){
                std::cout << "Дочерний в цикле....\n";
            }
            s+=r;
        }
        else{
            //wait(nullptr);
            std::cout << "Родитель: pid=" << getpid() << " ppid=" << getppid() << std::endl;
            for (int i=0; i<100000000; ++i){}
            for (int j=0; j<4; ++j){
                std::cout << "Родитель в цикле....\n";
            }
            k+=r;
        }
    }

    std::cout << "Final k = " << k << " s = "<< s << "\n";
    std::cout << "Process pid=" << getpid()<< " finish!!!\n";
    exit(1);
}
