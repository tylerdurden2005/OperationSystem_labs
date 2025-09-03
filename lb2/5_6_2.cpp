#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <pthread.h>
int pri[3] = {80, 10, 99};

void* threadFunction(void* arg) {
    int id = *(int*)arg;
    struct sched_param param;
    int policy;
    pthread_getschedparam(0, &policy, &param);
    for (int i = 0; i < 5; i++) {
    	for (int j=0; j<1000000000; ++j){}
        std::cout << "Поток №" << id << " приоритет=" << pri[id-1] << " шаг " << i << "\n";
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
    struct sched_param param;
    param.sched_priority = 50; 
    pthread_t t1, t2, t3;
    int id1 = 1, id2 = 2, id3 = 3;
    
    pthread_create(&t1, nullptr, threadFunction, &id1);
    pthread_create(&t2, nullptr, threadFunction, &id2);
    pthread_create(&t3, nullptr, threadFunction, &id3);
    param.sched_priority = pri[0]; 
    pthread_setschedparam(t1, SCHED_RR, &param);
    param.sched_priority = pri[1]; 
    pthread_setschedparam(t2, SCHED_RR, &param);
    param.sched_priority = pri[2]; 
    pthread_setschedparam(t3, SCHED_RR, &param);	
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    return 0;
}
