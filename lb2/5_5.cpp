#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/resource.h>

void setScheduler(size_t id) {
    
    if (id==0){
    	struct sched_param param;
    	param.sched_priority = 1;
    	if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
           exit(EXIT_FAILURE);
    	}
    }
    else if (id==1){
    	struct sched_param param;
    	param.sched_priority = 99;
    	if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
           exit(EXIT_FAILURE);
    	}
    }
}

int main() {
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1){
    	exit(EXIT_FAILURE);
    }
    
    /*struct sched_param param;
    param.sched_priority = 50;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        exit(EXIT_FAILURE);
    }*/
    pid_t pid1;
    if ((pid1 = fork()) == 0){
    	setScheduler(0);
    	for (int j = 0; j < 5; ++j) {
        	for (int j=0; j<1000000000; ++j){}
        	std::cout << "Процесс №" << 1 << " шаг " << j << "\n";
        	//std::cout << i;
        	std::cout.flush();
    	}
        exit(0);
    }
    pid_t pid2;
    if ((pid2 = fork()) == 0){
    	setScheduler(1);
    	for (int j = 0; j < 5; ++j) {
        	for (int j=0; j<1000000000; ++j){}
        	std::cout << "Процесс №" << 2 << " шаг " << j << "\n";
        	//std::cout << i;
        	std::cout.flush();
        	//sched_yield();
    	}
        exit(0);
    }
    pid_t pid3;
    if ((pid3 = fork()) == 0){
    	//setScheduler(1);
    	/*if (setpriority(PRIO_PROCESS, 0, -20) == -1) {
        	exit(EXIT_FAILURE);
    	}*/
    	for (int j = 0; j < 5; ++j) {
        	for (int j=0; j<1000000000; ++j){}
        	std::cout << "Процесс №" << 3  << " шаг " << j << "\n";
        	//std::cout << i;
        	std::cout.flush();
        	//usleep(100);
    	}
        exit(0);
    }
    
    
    while (wait(nullptr) > 0);
    return 0;
}
