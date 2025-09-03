#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
int main()
{
    std::cout << "Родитель: pid="<<getpid()<<" ppid="<<getppid() << "\n";
    int pid[3];
    if((pid[0] = fork()) == 0)
	execl("son1", "son1", nullptr);
    if((pid[1] = fork()) == 0)
	execl("son2", "son2", nullptr);
    if((pid[2] = fork()) == 0)
	execl("son3", "son3", nullptr);
    system("ps -s > 3_1before.txt");
    kill(pid[0], SIGUSR1);
    kill(pid[1], SIGUSR1);
    kill(pid[2], SIGUSR1);
    //sleep(5); 
    system("ps -s > 3_1after.txt");
    waitpid(pid[0], nullptr, 0);
    waitpid(pid[1], nullptr, 0);
    waitpid(pid[2], nullptr, 0);
   
    //system("ps -s >> 3_1after.txt");
    return 0;
}
