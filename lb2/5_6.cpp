#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/resource.h>


void* threadFunction(void* arg){
	return nullptr;
}
int main(){
    pthread_t t0;
    pthread_create(&t0, nullptr, threadFunction, nullptr);
    int policy;
    struct sched_param param;
    pthread_getschedparam(t0, &policy, &param);
    std::cout << "Планирование по умолчанию: ";
    switch(policy) {
        case SCHED_OTHER: std::cout <<"SCHED_OTHER\n"; break;
        case SCHED_FIFO:  std::cout <<"SCHED_FIFO\n";  break;
        case SCHED_RR:    std::cout <<"SCHED_RR\n";    break;
        default:          std::cout << "Неизвестно!\n";
    }
    std::cout << "Приоритет: " << param.sched_priority;
    int nice = getpriority(PRIO_PROCESS, 0);
    std::cout << " nice = " << nice << "\n";
}
