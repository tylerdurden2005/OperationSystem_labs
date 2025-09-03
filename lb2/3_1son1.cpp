#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

int main(){
   signal(SIGUSR1, SIG_DFL);
   std::cout << "son1 pid="<<getpid()<<" отреагировал на сигнал по умолчанию!\n";
   sleep(1);
   return 0;
}
