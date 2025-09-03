#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <pthread.h>
/*int pri[5] = {1, 99, 20, 30, 90};
void setScheduler(size_t id) {
    struct sched_param param;
    param.sched_priority = pri[id];
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        exit(EXIT_FAILURE);
    }
}*/
void* threadFunction(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; i++) {
    	for (int j=0; j<1000000000; ++j){}
        std::cout << "Поток №" << id << " шаг " << i << "\n";
    }
    return nullptr;
}

int main() {
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1){
    	exit(EXIT_FAILURE);
    }
    pthread_t t1, t2, t3;
    int id1 = 1, id2 = 2, id3 = 3;
    struct sched_param param;
    param.sched_priority = 50; 
    pthread_create(&t1, nullptr, threadFunction, &id1);
    pthread_create(&t2, nullptr, threadFunction, &id2);
    pthread_create(&t3, nullptr, threadFunction, &id3);
    //pthread_setschedparam(t1, SCHED_FIFO, &param);
    //pthread_setschedparam(t2, SCHED_FIFO, &param);
    //pthread_setschedparam(t3, SCHED_FIFO, &param);	
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    return 0;
}
