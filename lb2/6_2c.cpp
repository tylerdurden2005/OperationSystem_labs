#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sched.h>
#include <sys/wait.h>
#include <fcntl.h> 
int main(int argc, char *argv[]) {
    int num = std::stoi(argv[1]); 
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
    std::cout << " Ребенок execl №" << num << " политика: " << msg << " приоритет = " << param.sched_priority <<"\n";
	
    return 0;
}
