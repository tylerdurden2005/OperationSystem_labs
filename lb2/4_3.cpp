#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <csignal>
#include <sys/wait.h>
#include <vector>
int global = 0;
int* heap;
FILE* file;

void* threadFunction(void* arg){

    pid_t tid = syscall(SYS_gettid);
    std::cout << "Hello World! I am thread №" << *(int*)arg << " my tid = " << tid << std::endl;
    int local = 50;
    std::cout << "Адрес локальной переменной потока: " << (void*)&local << "\n";
    for (int i = 0; i < 10000; i++) {
        global++;
        (*heap)++;
    }
    std::string s =  "Hello World! I am thread №" + std::to_string(*(int*)arg) + " my tid = " + std::to_string(tid) + "\n";
    if (*(int*)arg == 1){
        file = fopen("4_3.txt", "w");
        fprintf(file, "%s", s.c_str());
    }
    else{
        fprintf(file, "%s", s.c_str());
        fclose(file);
    }
    sleep(5);
    return nullptr;
}
int main(){
    heap = new int;
    *heap = 0;
    pthread_t thread1;
    int arg1 =1;
    int arg2 = 2;
    pthread_create(&thread1, nullptr, threadFunction, &arg1);
    pthread_t thread2;
    sleep(1);
    pthread_create(&thread2, nullptr, threadFunction, &arg2);
    sleep(2);
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    std::cout << "Global: " << global << "\n";
    std::cout << "Heap: " << *heap << "\n";
    delete heap;
    return 0;
}
