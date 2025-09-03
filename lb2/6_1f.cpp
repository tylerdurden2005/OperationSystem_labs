#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sched.h>
#include <sys/wait.h>
#include <fcntl.h> 
void setScheduler() {
    struct sched_param param;
    param.sched_priority = 50;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        exit(EXIT_FAILURE);
    }
}

int main() {
    /*struct sched_param param;
    param.sched_priority = 50;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        exit(EXIT_FAILURE);
    }*/
    umask(0); 
    int file = open("6_1res.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    std::string par = "Родитель записан\n";
    write(file, par.c_str(), par.size());
    for (int i = 0; i < 3; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            //setScheduler();
            /*std::string msg = "Ребёнок №" + std::to_string(i+1) +" записан\n";
            write(file, msg.c_str(), msg.size());
            exit(0);*/
            std::string arg1 = std::to_string(i + 1);
            std::string arg2 = std::to_string(file);
            execl("./6_1c", "6_1c", arg1.c_str(), arg2.c_str(), nullptr);
        }
    }
    while (wait(nullptr) > 0);
    close(file);
    return 0;
}
