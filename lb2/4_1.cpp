#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <csignal>
#include <sys/wait.h>
#include <vector>

void* threadFunction(void* arg){
    pid_t tid = syscall(SYS_gettid);
    std::cout << "Hello World! I am thread №" << *(int*)arg << " my tid = " << tid << std::endl;
    sleep(5);
    return nullptr;
}
void firstPart(){
    pthread_t threads[5];
    int ids[5] = {1, 2, 3, 4, 5};
    system("ps -Lf > 4_1before.txt");
    for (size_t i=0; i<5; ++i){
        sleep(1);
        pthread_create(&threads[i], nullptr, threadFunction, &ids[i]);
    }
    system("ps -Lf > 4_1after.txt");
    for (pthread_t& thread : threads) {
        pthread_join(thread, nullptr);
    }
}
int cloneFunction(void* arg){
    pid_t tid = syscall(SYS_gettid);
    std::cout << "Hello World from CLONE №" << *(int*)arg << " tid = " << tid << std::endl;
    sleep(5);
    return 0;
}
void secondPart(){
    char* stack = (char*)malloc(1024 * 1024);
    int ids[5] = {1, 2, 3, 4, 5};
    std::vector<pid_t> cpids;
    system("ps -Lf > 4_1beforeclone.txt");
    for (size_t i=0; i<5; ++i){
        sleep(1);
        //pid_t cpid = clone(cloneFunction, stack + (1024*1024), CLONE_VM | SIGCHLD, &ids[i]);
        pid_t cpid = clone(cloneFunction, stack + (1024*1024), CLONE_THREAD | CLONE_SIGHAND| CLONE_VM , &ids[i]);
        cpids.push_back(cpid);
    }
    system("ps -Lf > 4_1afterclone.txt");
    for (auto& i : cpids){
        waitpid(i, nullptr, 0);
    }
}
int main(){
    std::cout << "Создание потоков при помощи pthrea_create():\n\n";
    firstPart();
    std::cout << "\n\nСоздание потоков при помощи clone():\n\n";
    secondPart();
}
