/*Ввод сообщений в очередь для сервера*/

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <iostream>
#include <fcntl.h>     
#include <sys/stat.h> 
#include <unistd.h>    
#include <cstring>  

#define BUF_SIZE 256
#define KEY "msgqueue"

struct message {
    long mtype;
    pid_t pid;
    char text[BUF_SIZE];
};

int main(){
    key_t key = ftok(KEY, 65);  // Генерация уникального ключа
    int queue = msgget(key, IPC_CREAT | 0666); // Получение ID очереди

    struct message reply;
    struct message msg;
    
    while (true) {
        memset(&msg, 0, sizeof(msg));
        msg.mtype = 99; 
    	msg.pid = getpid();
        std::cout << "Введите сообщение серверу: ";
        fgets(msg.text, BUF_SIZE, stdin);
	msg.text[strlen(msg.text) - 1] = '\0';
	if (strcmp(msg.text, "exit") == 0) {
            std::cout << "Завершение клиента pid = " << getpid() << std::endl;
            break;
        }
        msgsnd(queue, &msg, sizeof(msg) - sizeof(long), IPC_NOWAIT); // отправка сообщения
        memset(&reply, 0, sizeof(reply));
        if (msgrcv(queue, &reply, sizeof(reply) - sizeof(long), getpid(), IPC_NOWAIT) < 0) {
            if (errno == ENOMSG) {
        	std::cout << "Пока нет ответа от сервера. Попробуйте позже." << std::endl;
        	continue;
    	    } 
            std::cout << "Сообщение не получено сервером... ошибка" << std::endl;
            break;
        }
        std::cout << "Ответ сервера: " << reply.text << std::endl;
    }

    return 0;
}

