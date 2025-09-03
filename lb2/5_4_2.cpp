#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
int pri[5] = {1, 99, 20, 30, 90};
void setScheduler(size_t id) {
    struct sched_param param;
    param.sched_priority = pri[id];
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        exit(EXIT_FAILURE);
    }
}

int main() {
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1){
    	exit(EXIT_FAILURE);
    }
    
    struct sched_param param;
    param.sched_priority = 50;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        exit(EXIT_FAILURE);
    }
   
    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            setScheduler(i);
           
            for (int j = 0; j < 5; ++j) {
        	
        	for (int j=0; j<1000000000; ++j){}
        	std::cout << "Процесс №" << i << " шаг " << j << "\n";
        	//std::cout << i;
        	std::cout.flush();
        	//sched_yield();
        	//usleep(10);
    	    }
            exit(0);
        }
    }
    
    while (wait(nullptr) > 0);
    return 0;
}
