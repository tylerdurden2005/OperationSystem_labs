#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <csignal>
#include <sys/wait.h>
#include <vector>
pid_t tids[2];

void handler(int signum) {
    std::cout << "Поток получил SIGTERM. Завершаюсь...\n";
    pthread_exit(nullptr);
}
void* threadFunction(void* arg){
    signal(SIGTERM, handler);
    pid_t tid = syscall(SYS_gettid);
    int id = *(int*)arg;
    tids[id-1] = tid;
    std::cout << "Hello World! I am thread №" << *(int*)arg << " my tid = " << tid << std::endl;
    sleep(10);
    return nullptr;
}
int main(){
    pthread_t thread1;
    int arg1 =1;
    int arg2 = 2;
    pthread_create(&thread1, nullptr, threadFunction, &arg1);
    pthread_t thread2;
    sleep(1);
    pthread_create(&thread2, nullptr, threadFunction, &arg2);
    sleep(2);
    system("ps -Lf > 4_4before.txt");
    if (kill(tids[0], SIGTERM) == 0) {
        std::cout << "Сигнал SIGTERM отправлен потоку.\n";
    }
    else {
        std::cout << "Ошибка отправки сигнала!";
    }
    system("ps -Lf > 4_4after.txt");
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    return 0;
}
