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
    else if (pid == 0){
        std::cout << "Дочерний процесс: pid=" << getpid() << " ppid=" << getppid()<<std::endl;
        for (int i =0; i<10; ++i){
            std::cout << "Дочерний процесс.....\t\t";
            for (int j=0; j<1000000; ++j){}
            s+=i;
            std::cout << "s: " << s << "\n";
        }
    }
    else{
    	//wait(nullptr);
        std::cout << "Родитель: pid=" << getpid() << " ppid=" << getppid() << std::endl;
        for (int i =0; i<10; ++i){
            std::cout << "Родительский процесс....\t";
            for (int j=0; j<1000000; ++j){}
            k+=i;
            std::cout << "k: " << k << "\n";
        }
    }
    std::cout << "Final k = " << k << " s = "<< s << "\n";
    std::cout << "Process pid=" << getpid()<< " finish!!!\n";
    exit(1);
}
