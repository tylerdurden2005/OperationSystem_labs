/*Сервер для получения сообщений из очереди*/

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <iostream>
#include <fcntl.h>     
#include <signal.h> 
#include <cstring>


#define BUF_SIZE 256
#define KEY "msgqueue"
struct message {
    long mtype; // тип сообщения 
    pid_t pid;
    char text[BUF_SIZE];
};

int queue;

void handler(int sig) {
    signal(sig, SIG_DFL);
    msgctl(queue, IPC_RMID, 0); // корректное удаление очереди
}

int main(){
    key_t key = ftok(KEY, 65);  // Генерация уникального ключа
    queue = msgget(key, IPC_CREAT | 0666); // создание очереди сообщений
    signal(SIGINT, handler);

    struct message msg;
    while (true) {
        memset(&msg, 0, sizeof(msg));

        if (msgrcv(queue, &msg, sizeof(msg) - sizeof(long), 99, 0) < 0) { // прием сообщения
            if (errno == EINTR){
            	kill(getpid(), SIGINT);
            }
            else{
               std::cout << "Сообщения не получены!" << std::endl;
               kill(getpid(), SIGINT);
            }
        }

        std::cout << "От клиента " << msg.pid << " получено сообщение: " << msg.text << std::endl;

        message reply; // ответное сообщение
        memset(&reply, 0, sizeof(reply));
        reply.mtype = msg.pid;
        reply.pid = getpid(); 
        strcpy(reply.text,"Сообщение получено!");
        msgsnd(queue, &reply, sizeof(reply) - sizeof(long), 0); // отправка сообщения
    }

    return 0;
}
