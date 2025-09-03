/* fifo  писатель*/

#include <iostream>
#include <fcntl.h>     
#include <sys/stat.h> 
#include <unistd.h>    
#include <cstring>  

#define FIFO_NAME "customFIFO"

int main(){
    mkfifo(FIFO_NAME, 0666);
    int fd = open(FIFO_NAME, O_WRONLY);
    const char *messages[] = {
        "Привет!",
        "Как дела?",
        "Это третье сообщение.",
        "Это четвертое сообщение.",
        "Это пятое сообщение сообщение.",
        "До свидания!"
    };
    for (size_t i=0; i<6; ++i){
    	write(fd, messages[i], strlen(messages[i]));
        sleep(1);
    }
    close(fd);
    // unlink(FIFO_NAME);
}
