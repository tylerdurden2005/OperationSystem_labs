#include <iostream>
#include <sched.h>
#include <sys/resource.h>
#include <csignal>
#include <unistd.h>
#include <wait.h>


int main() {
    int min1 = sched_get_priority_min(SCHED_FIFO);
    int max1 = sched_get_priority_max(SCHED_FIFO);
    std::cout << "Диапазон SCHED_FIFO: " << min1 << " - " << max1 << "\n";
    int min2 = sched_get_priority_min(SCHED_RR);
    int max2 = sched_get_priority_max(SCHED_RR);
    std::cout << "Диапазон SCHED_RR: " << min2 << " - " << max2 << "\n";
    int min3 = sched_get_priority_min(SCHED_OTHER);
    int max3 = sched_get_priority_max(SCHED_OTHER);
    std::cout << "Диапазон SCHED_OTHER: " << min3 << " - " << max3 << "\n";
    std::cout << "Диапазон nice: " << PRIO_MIN << " - " << PRIO_MAX-1 << "\n";
    sched_param param;
    param.sched_priority = min1 - 1;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        std::cout << "Ошибка SHED_FIFO: приоритет ниже минимума\n";
    }
    param.sched_priority = max1 + 1;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        std::cout << "Ошибка SHED_FIFO: приоритет выше максимума\n";
    }
    param.sched_priority = min2 - 1;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        std::cout << "Ошибка SHED_RR: приоритет ниже минимума\n";
    }
    param.sched_priority = max2 + 1;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        std::cout << "Ошибка SHED_RR: приоритет выше максимума\n";
    }
    param.sched_priority = min3 - 1;
    if (sched_setscheduler(0, SCHED_OTHER, &param) == -1) {
        std::cout << "Ошибка SHED_OTHER: приоритет ниже минимума\n";
    }
    param.sched_priority = max3 + 1;
    if (sched_setscheduler(0, SCHED_OTHER, &param) == -1) {
        std::cout << "Ошибка SHED_OTHER: приоритет выше максимума\n";
    }
}
