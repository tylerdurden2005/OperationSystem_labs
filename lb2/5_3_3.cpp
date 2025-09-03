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
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);
    struct sched_param param;
    param.sched_priority = 50;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            setScheduler(i);
            std::string arg = std::to_string(i + 1);
            for (int j = 0; j < 5; ++j) {
        	std::cout << "Процесс №" << i << " приоритет = " << pri[i] << " шаг " << j << "\n";
        	std::cout.flush(); 
    	    }
            exit(0);
        }
    }
    while (wait(nullptr) > 0);
    return 0;
}
