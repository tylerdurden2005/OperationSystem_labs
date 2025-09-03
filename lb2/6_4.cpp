#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sched.h>
#include <sys/mman.h>
#include <csignal>
#include <sys/wait.h>
#include <fcntl.h> 
#include <vector>

int cloneFunction(void* arg){
    //pid_t tid = syscall(SYS_gettid);
    
    int policy = sched_getscheduler(0); 
    struct sched_param param;
    sched_getparam(0, &param);
    std::string msg;
    switch (policy) {
	case SCHED_FIFO:
		msg = "SCHED_FIFO";
   		break;
	case SCHED_RR:
		msg = "SCHED_RR";
		break;

	case SCHED_OTHER:
		msg = "SCHED_OTHER";
		break;
   	default:
   		msg = "Неизвестная политика планирования";

    } 
    std::cout << "Hello World from CLONE №" << *(int*)arg << " polyci = " << msg << " prioritet = " << param.sched_priority << std::endl;
    sleep(2);
    return 0;
}
int main() {
    struct sched_param param;
    param.sched_priority = 99;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        exit(EXIT_FAILURE);
    }
    char* stack = new char[1024 * 1024];
    std::vector<pid_t> cpids;
    int ids[5] = {1, 2, 3, 4, 5};
    for (size_t i=0; i<3; ++i){
        sleep(1);
        pid_t cpid = clone(cloneFunction, stack + (1024*1024), CLONE_THREAD | CLONE_SIGHAND| CLONE_VM , &ids[i]);
        cpids.push_back(cpid);
    }
    
    for (auto& i : cpids){
        waitpid(i, nullptr, 0);
    }
    sleep(2);
    delete[] stack;
    stack = nullptr; 
    return 0;
}
