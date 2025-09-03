#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
int main(int argc, char *argv[]) {
    int num = std::stoi(argv[1]); 
    int file = std::stoi(argv[2]); 
    std::string msg = " Ребенок execl №" + std::to_string(num) + "\n";
    write(file, msg.c_str(), msg.size());
    return 0;
}
