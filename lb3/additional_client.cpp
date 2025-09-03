/*Клиент, который в зависимости от события посылает сообщение*/

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <iostream>
#include <fcntl.h>     
#include <sys/stat.h> 
#include <unistd.h>    
#include <cstring>  
#include <signal.h> 

#define BUF_SIZE 512
#define KEY "msgqueue"
#define EVENT_LOGIN        1
#define EVENT_DATA_REQUEST 2
#define EVENT_LOGOUT       3
#define EVENT_CONNECT      4
#define EVENT_HELLO        5
#define EVENT_CL_HELLO     6
#define EVENT_ERROR_LOGOUT 99

struct message {
    long mtype;
    pid_t pid;
    char text[BUF_SIZE];
};

int queue;
bool flag = true; // отслеживать завершение работы клиента

void handle(int sig) { // обработка SIGIN SIGTSTP - отправить сообщение на сервер с типом EVENT_ERROR_LOGOUT и завершить все
    message msg;
    msg.mtype = EVENT_ERROR_LOGOUT;
    msg.pid = getpid();
    strcpy(msg.text, "error");
    msgsnd(queue, &msg, sizeof(msg) - sizeof(long), 0);
    flag = false;
    exit(0);
}

void info(){ // просто информация для пользоателя
    std::cout << "\t --- \'request\' - запрос данных от сервера\n"
                 "\t --- \'ping\' - проверка подключения к серверу\n"
                 "\t --- \'hello\' - передать сообщение первому клиенту (например первый это админ)\n"
                 "\t --- \'exit\' - завершить сеанс\n";
}

void* receive_messages(void* arg) { 
    message msg;
    while (flag) {
        if (msgrcv(queue, &msg, sizeof(msg) - sizeof(long), EVENT_CL_HELLO, 0) >= 0 && msg.pid == getpid()) {
            std::cout << "\nСообщение от клиента: " << msg.text << std::endl << std::endl;
        }
    }
    return nullptr;
}

int main(){
    key_t key = ftok(KEY, 65);  // Генерация уникального ключа
    queue = msgget(key, IPC_CREAT | 0666); // Получение ID очереди
    
    
    signal(SIGINT, handle);
    signal(SIGTSTP, handle);
    
    struct message reply;
    struct message msg;
    
    msg.mtype = EVENT_LOGIN;
    msg.pid = getpid();
    strcpy(msg.text, "Login");
    msgsnd(queue, &msg, sizeof(msg) - sizeof(long), 0); // отправка сообщения, что пользователь появился (EVENT_LOGIN)
    if(msgrcv(queue, &reply, sizeof(reply) - sizeof(long), getpid(), 0) >=0 ){ // ждем сообщение от сервера
    	info();
    	
    	pthread_t recv_thread; // поток для чтения сообщения от клиента другого
    	pthread_create(&recv_thread, nullptr, receive_messages, nullptr);
    	pthread_detach(recv_thread); // поток становится "самоуправляемым" — после завершения он исчезает без следа
    	
    	std::cout << "Ответ сервера: " << reply.text << std::endl;
    	
    	while (flag) { 
    		
        	memset(&msg, 0, sizeof(msg));
        	memset(&reply, 0, sizeof(reply));
        	std::cout << "";
        	std::cout << "Введите сообщение серверу: ";
        	fgets(msg.text, BUF_SIZE, stdin);
		msg.text[strlen(msg.text) - 1] = '\0'; // команда пользователя 
		if (strcmp(msg.text, "exit") == 0) {
			msg.pid = getpid();
			msg.mtype = EVENT_LOGOUT;
            		std::cout << "Завершение клиента pid = " << getpid() << std::endl;
            		msgsnd(queue, &msg, sizeof(msg) - sizeof(long), 0);
            		flag = false;
            		break;
            	}
            	else if (strcmp(msg.text, "ping") == 0){
            		msg.pid = getpid();
			msg.mtype = EVENT_CONNECT;
            		msgsnd(queue, &msg, sizeof(msg) - sizeof(long), 0);
            		if (msgrcv(queue, &reply, sizeof(reply) - sizeof(long), getpid(), 0) >= 0){
            			std::cout << "Соединение стабильно, ответ сервера: " << reply.text << std::endl;
            		}
            		
            	}
            	else if (strcmp(msg.text, "request") == 0){
            		msg.pid = getpid();
			msg.mtype = EVENT_DATA_REQUEST;
            		msgsnd(queue, &msg, sizeof(msg) - sizeof(long), 0);
            		if (msgrcv(queue, &reply, sizeof(reply) - sizeof(long), getpid(), 0) >= 0){
            			std::cout << "Сервер выдал данные: " << reply.text << std::endl;
            		}
            		
            	}
            	else if (strcmp(msg.text, "hello") == 0){
            		memset(&msg, 0, sizeof(msg));
            		msg.pid = getpid();
			msg.mtype = EVENT_HELLO;
			std::cout << "Введите сообщение клиенту: ";
        		fgets(msg.text, BUF_SIZE, stdin);
			msg.text[strlen(msg.text) - 1] = '\0';
            		msgsnd(queue, &msg, sizeof(msg) - sizeof(long), 0);
            		
            	}
        }
      
    }
    
    return 0;
}

