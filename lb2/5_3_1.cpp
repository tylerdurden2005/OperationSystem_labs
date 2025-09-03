#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
int pri[5] = {10, 99, 8, 20, 90};
void setScheduler(size_t id) {
    struct sched_param param;
    param.sched_priority = pri[id];
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        exit(EXIT_FAILURE);
    }
}

int main() {
    struct sched_param param;
    param.sched_priority = 50;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        exit(EXIT_FAILURE);
    }
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);
   
    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == 0) {
           // setScheduler(i);
           
            for (int j = 0; j < 5; ++j) {
            	//for (int j=0; j<10000000000; ++j){}
        	//std::cout << i;
        	std::cout << "Процесс №" << i << " шаг " << j << "\n";
        	std::cout.flush(); 
    	    }
            exit(0);
        }
    }
    while (wait(nullptr) > 0);
    return 0;
}
