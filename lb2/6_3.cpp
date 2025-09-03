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

static void handler(int sig) {
	signal(sig, SIG_DFL);
	std::string msg;
	if (sig == SIGUSR1) msg = "SIGUSR1";
	else msg = "SIGUSR2";
	std::cout << "Процесс pid = " << getpid() << " ppid = " << getppid() << " поймал сигнал: " << msg << "\n";
	return;
}	

int main() {
	std::cout << "Родитель pid = " << getpid() << "\n";
	signal(SIGUSR1, handler);
 	signal(SIGUSR2, handler);
	int pid[3];
    	if((pid[0] = fork()) == 0){
    		std::cout << "Ребенок pid = " << getpid() << " ppid = " << getppid() << "\n";

		while(1) pause();
		exit(0);	
    	}

    	if((pid[1] = fork()) == 0){
    		std::cout << "Ребенок pid = " << getpid() << " ppid = " << getppid() << "\n";

		while(1) pause();
		exit(0);
    	}
 
    	if((pid[2] = fork()) == 0){
    		std::cout << "Ребенок pid = " << getpid() << " ppid = " << getppid() << "\n";

		while(1) pause();
		exit(0);
    	}
	usleep(100000);
    	//kill(pid[0], SIGUSR1);
    	//kill(pid[1], SIGUSR2);
    	//kill(pid[2], SIGUSR1);

    	waitpid(pid[0], nullptr, 0);
    	waitpid(pid[1], nullptr, 0);
    	waitpid(pid[2], nullptr, 0);
    	while(1) pause();
	return 0;
}
