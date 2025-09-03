/*Программа реализует pipe как межпроцессорное взаимодействие дочернего процесса с родителем*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iomanip>
#include <cstring>

#define BUF_SIZE 256

int main(){
    int fd[2];
    char c;
    pipe(fd);
    pid_t cpid = fork();
    if (cpid == 0) {
         close(fd[1]);
         char buffer[BUF_SIZE];
         ssize_t bytesRead;

         while ((bytesRead = read(fd[0], buffer, BUF_SIZE - 1)) > 0) {
            buffer[bytesRead] = '\0'; 
            std::cout << "Дочерний получил: " << buffer << std::endl;
         }
         close(fd[0]);
         exit(0);
    } 
    else {
         sleep(1); 
    	 close(fd[0]);
    	 const char* messages[] = {
            "Привет из родителя!",
            "Работаем с pipe",
            "Передаём строки",
            "Идет передача...",
            "Hello from parrent!",
            "Working with pipe",
            "Passing strings",
            "Transmission is in progress...",
            "Пока!",
            "Bye!"
         };
    	 for (size_t i =0; i<10; ++i){
    	      write(fd[1], messages[i], strlen(messages[i]));
              sleep(1);
    	 }
    	 close(fd[1]);
    	 wait(nullptr);
    	 exit(0);
    }
}
