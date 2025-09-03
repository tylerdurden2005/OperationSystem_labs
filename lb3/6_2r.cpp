/*fifo читатель*/

#include <iostream>
#include <fcntl.h>      
#include <sys/stat.h>   
#include <unistd.h>     
#include <cstring>  

#define FIFO_NAME "customFIFO"
#define BUF_SIZE 256
int main(){
    char buffer[BUF_SIZE];
    int fd = open(FIFO_NAME, O_RDONLY);
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, BUF_SIZE - 1)) > 0) {
        buffer[bytesRead] = '\0'; 
        std::cout << "Читатель получил: " << buffer << std::endl;
    }
    close(fd);
    return 0;
}
