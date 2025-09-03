#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring> 
#include <sys/wait.h>

#define PORT 8080
#define BUF_SIZE 256

int main(){
    std::cout << "Server SOMAXCONN\n";
    int server_fd, new_socket; 
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // создание нового сокета
    
    struct sockaddr_in address; // эта структура содержит интернет адрес
    address.sin_family = AF_INET; // код семейства адресов
    address.sin_addr.s_addr = INADDR_ANY; // ip адрес хоста (ip  компьютера) - константа INADDR_ANY, которая получает этот адрес
    address.sin_port = htons(PORT); // номер порта при помощи htons преобразуется в сетевой порядок байтов
    
    bind(server_fd, (struct sockaddr*)&address, sizeof(address)); // привязывает сокет к адресу текущего хоста и номеру порта
    listen(server_fd, SOMAXCONN); // слушать (ожидать) запросы от клиентов; можно использовать SOMAXCONN - максимально допустимую очередь подключений
    
    struct sockaddr_in cli_address;
    socklen_t addrlen = sizeof(cli_address);
    
    char buffer[BUF_SIZE];
    std::string reply = "Сервер получил сообщение!";
    
    while(true){
    	new_socket = accept(server_fd, (struct sockaddr*)&cli_address, &addrlen); // принять запрос от клиента, блокировка процесса до тех пор, пока клиент не подключится к серверу
    	pid_t pid = fork();
    	if (pid == 0) {
            // В дочернем процессе обрабатываем клиента
            close(server_fd); // серверный сокет закрывается в дочернем процессе

            memset(buffer, 0, BUF_SIZE);
            ssize_t bytes_read = read(new_socket, buffer, BUF_SIZE - 1); // сообщение клиента
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0'; 
                std::cout << "Сообщение от клиента: " << buffer << std::endl;
                write(new_socket, reply.c_str(), reply.length()); // отправляем ответ
            }

            close(new_socket); // закрываем соединение с клиентом
            exit(0); 
        }
        else{
         close(new_socket);
         //waitpid(pid, nullptr, 0);
        }
    }
    
    close(server_fd);
}
