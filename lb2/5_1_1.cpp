#include <iostream>
#include <sched.h>
#include <sys/resource.h>

int main() {
    int policy = sched_getscheduler(0);
    std::cout << "Планирование по умолчанию: ";
    switch(policy) {
        case SCHED_OTHER: std::cout <<"SCHED_OTHER\n"; break;
        case SCHED_FIFO:  std::cout <<"SCHED_FIFO\n";  break;
        case SCHED_RR:    std::cout <<"SCHED_RR\n";    break;
        default:          std::cout << "Неизвестно!\n";
    }
    struct sched_param param;
    sched_getparam(0, &param);
    std::cout << "Приоритет: " << param.sched_priority;
    int nice = getpriority(PRIO_PROCESS, 0);
    std::cout << " nice = " << nice << "\n";

    return 0;
}
