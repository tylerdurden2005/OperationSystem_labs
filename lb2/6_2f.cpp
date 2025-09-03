#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sched.h>
#include <sys/wait.h>
#include <fcntl.h> 

int main() {
    struct sched_param param;
    param.sched_priority = 99;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 3; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            /*std::string msg = "Ребёнок №" + std::to_string(i+1) +" записан\n";
            write(file, msg.c_str(), msg.size());
            exit(0);*/
            std::string arg1 = std::to_string(i + 1);
            execl("./6_2c", "6_2c", arg1.c_str(), nullptr);
        }
    }
    while (wait(nullptr) > 0);
    return 0;
}
