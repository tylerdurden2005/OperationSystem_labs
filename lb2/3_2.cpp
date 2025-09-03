#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
int main()
{
    std::cout << "Родитель: pid="<<getpid()<<" ppid="<<getppid() << "\n";
    int pid[5];
    for (int i=0; i<5; ++i){
        pid[i] = fork();
        if (pid[i] == 0) {
            std::cout << "Дочерний процесс №" << i << " pid=" << getpid() <<" ppid=" << getppid()<<"\n";
            sleep(i + 1);
            exit(i + 1);
        }
    }
    system("ps -s > 3_2info.txt");
    for (int i = 0; i < 5; ++i) {
        int status;
        int p = waitpid(pid[i], &status, WUNTRACED);
        if (p > 0 && WIFEXITED(status)) {
		std::cout << "Дочерний процесс №" << i << " pid=" << p <<" ppid=" << getppid()<<" завершился с exit status: " << WEXITSTATUS(status)<<"\n";
        }
    }
    return 0;
}
