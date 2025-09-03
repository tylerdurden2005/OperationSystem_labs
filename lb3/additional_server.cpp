/*Сервер для получения сообщений по событию.*/

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <iostream>
#include <fcntl.h>     
#include <signal.h> 
#include <cstring>
#include <algorithm>

#define BUF_SIZE 512
#define KEY "msgqueue"
#define EVENT_LOGIN        1 // процесс клиента был запущен, т.е. ./client - это как login
#define EVENT_DATA_REQUEST 2 // клиент запросил данные
#define EVENT_LOGOUT       3 // клиент закрылся при помощи exit 
#define EVENT_CONNECT      4 // клиент проверяет соединение
#define EVENT_HELLO        5 // передача сообщению другому клиенту
#define EVENT_CL_HELLO     6 // передача сообщения уже сервером от клиента клиенту
#define EVENT_ERROR_LOGOUT 99 // как будто 'аварийный' выход клиента (у клиента сработаны сигналы SIGIN SIGTSTP)


struct message {
    long mtype; // тип сообщения 
    pid_t pid;
    char text[BUF_SIZE];
};

int queue;
std::vector<int> clients; // клиенты оналйн
size_t client_count = 0; // количество клиентов

void handler(int sig) { // обработка сигналов SIGIN SIGTSTP для сервера
    if (sig == SIGINT) signal(sig, SIG_DFL);
    else if (sig == SIGTSTP) signal (sig, SIG_DFL);
    msgctl(queue, IPC_RMID, 0); // корректное удаление очереди
    exit(0);
}

void add_client(pid_t pid) { // просто добавляем клиента в массив
    if (std::find(clients.begin(), clients.end(), pid) == clients.end()){
    	clients.push_back(pid);
    	client_count++;
    }
} 
void remove_client(pid_t pid) { // удаление клиента из массива
    auto it = std::find(clients.begin(), clients.end(), pid);
    if (it != clients.end()) {
        clients.erase(it);
        client_count--;
    }
}

int main(){
    key_t key = ftok(KEY, 65);  // Генерация уникального ключа
    queue = msgget(key, IPC_CREAT | 0666); // создание очереди сообщений
    signal(SIGINT, handler);
    signal(SIGTSTP, handler);
    std::cout << "Сервер начал свою работу." << std::endl;
    message msg;
    message reply;
    while (true) {
        memset(&msg, 0, sizeof(msg));
	memset(&reply, 0, sizeof(reply));
        if (msgrcv(queue, &msg, sizeof(msg) - sizeof(long), 0, 0) >= 0) { // прием сообщения
            long event_type = msg.mtype;
            int client_pid = msg.pid;
            
            switch(event_type){ // в зависимости от типа (события) 
                case EVENT_LOGIN:
     		    std::cout << "Клиент №" << client_pid << " авторизовался!" << std::endl;
     		    add_client(client_pid);
                    reply.mtype = client_pid; 
        	    reply.pid = client_pid; 
        	    strcpy(reply.text,"Успешная авторизация!");
        	    msgsnd(queue, &reply, sizeof(reply) - sizeof(long), 0); // обратная связь клиенту
                    break;
                case EVENT_DATA_REQUEST: {
                    std::cout << "Клиент №" << client_pid << " запросил данные!" << std::endl;
                    reply.mtype = client_pid; //
        	    reply.pid = client_pid; 
        	    strcpy(reply.text," ваши данные....");
        	    msgsnd(queue, &reply, sizeof(reply) - sizeof(long), 0); // обратная связь клиенту
        	    break;
                }
                case EVENT_HELLO: {
                    std::cout << "Сервер передает сообщение от пользователя: " << client_pid << std::endl;
                    
                    memset(&reply, 0, sizeof(reply));
                    strcpy(reply.text, msg.text);
                    reply.mtype = EVENT_CL_HELLO;
                    reply.pid = clients[0];
                    
                    msgsnd(queue, &reply, sizeof(reply) - sizeof(long), 0); // отправка сообщения первому клиенту из массива

                    break;
                }
                case EVENT_CONNECT:
                    std::cout << "Клиент №" << client_pid << " проверяет связь: " << msg.text << std::endl;
                    reply.mtype = client_pid; //
        	    reply.pid = client_pid; 
        	    strcpy(reply.text," PONG - соединение корректно!");
        	    msgsnd(queue, &reply, sizeof(reply) - sizeof(long), 0); // обратная связь клиенту
        	    break;
                case EVENT_LOGOUT:
                    std::cout << "Клиент №" << msg.pid << " отключился!" << std::endl;
                    remove_client(client_pid);
                    break;
                case EVENT_ERROR_LOGOUT:
                    std::cout << "Клиент №" << msg.pid << " - ошибка со стороны клиента!" << std::endl;
                    remove_client(client_pid);
                    break;
                default:
                    std::cout << "Неизвестное событие от пользователя pid = " << client_pid << std::endl;
            }
        }
    }
    
    std::cout << "Сервер завершил свою работу." << std::endl;
    return 0;
}
