#include <ctime>
#include <iomanip>
#include <sched.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    struct sched_param param;
    param.sched_priority = 50;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        exit(EXIT_FAILURE);
    }
    timespec tp;

    if (sched_rr_get_interval(0, &tp) == -1) {
        return 1;
    }
    std::cout << "SCHED_RR quantum: "  << std::fixed << std::setprecision(8) << tp.tv_sec + tp.tv_nsec / 1000000000.0 << " секунд "<< tp.tv_nsec << " наносекунд\n";

    double ms = tp.tv_sec * 1000.0 + tp.tv_nsec / 1000000.0;
    std::cout << "Или примерно: " << std::fixed << std::setprecision(3)  << ms << " миллисекунд\n";
    return 0;
}
