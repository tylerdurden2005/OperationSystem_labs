#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <signal.h>


int main(){
   signal(SIGUSR1, SIG_IGN);
   std::cout << "son2 pid="<<getpid()<<" проигнорировал сигнал!\n";
   sleep(1);
   return 0;
}
