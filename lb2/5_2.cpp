#include <iostream>
#include <sched.h>
#include <sys/resource.h>
#include <csignal>
#include <unistd.h>
#include <wait.h>


int main() {
    system("> 5_2.txt");
    int pid = getpid();
    int old = getpriority(PRIO_PROCESS, pid);
    std::cout << "Текущий приоритет: " << old << "\n";
    std::string s = "ps -o ni,pid,comm -p " + std::to_string(getpid()) + " >> 5_2.txt";
    system(s.c_str());

    sleep(10);
    
    int new1 = -19;
    if (setpriority(PRIO_PROCESS, pid, new1) == -1) {
        std::cout << "Ошибка!\n";
    }
    else {
        std::cout << "Приоритет изменен на "<< new1 << "\n";
    }
    std::cout << "getpriority() = " <<getpriority(PRIO_PROCESS, pid) << "\n";
    s = "ps -o ni,pid,comm -p " + std::to_string(getpid()) + " >> 5_2.txt";
    system(s.c_str());
    
    sleep(10);
    
    
    int new2 = 19;
    if (setpriority(PRIO_PROCESS, pid, new2) == -1) {
        std::cout << "Ошибка!\n";
    } 
    else {
        std::cout << "Приоритет изменен на "<< new2 << "\n";
    }
    std::cout << "getpriority() = " <<getpriority(PRIO_PROCESS, pid) << "\n";
    s = "ps -o ni,pid,comm -p " + std::to_string(getpid()) + " >> 5_2.txt";
    system(s.c_str());
    
    sleep(10);
    return 0;
}
