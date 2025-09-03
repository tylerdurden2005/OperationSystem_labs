/*Сервер для tcp (взаимодействие с 1 клиентом) */

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring> 

#define PORT 8080
#define BUF_SIZE 256

int main(){
    int server_fd, new_socket; 
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // создание нового сокета
    
    struct sockaddr_in address; // эта структура содержит интернет адрес
    address.sin_family = AF_INET; // код семейства адресов
    address.sin_addr.s_addr = INADDR_ANY; // ip адрес хоста (ip  компьютера) - константа INADDR_ANY, которая получает этот адрес
    address.sin_port = htons(PORT); // номер порта при помощи htons преобразуется в сетевой порядок байтов
    
    bind(server_fd, (struct sockaddr*)&address, sizeof(address)); // привязывает сокет к адресу текущего хоста и номеру порта
    listen(server_fd, 5); // слушать (ожидать) запросы от клиентов; можно использовать SOMAXCONN - максимально допустимую очередь подключений
    
    struct sockaddr_in cli_address;
    socklen_t addrlen = sizeof(cli_address);
    new_socket = accept(server_fd, (struct sockaddr*)&cli_address, &addrlen); // принять запрос от клиента, блокировка процесса до тех пор, пока клиент не подключится к серверу
    
    char buffer[BUF_SIZE];
    memset(buffer, 0, BUF_SIZE);
    std::string reply = "Сервер получил сообщение!";
    ssize_t bytes_read;
    while ((bytes_read = read(new_socket, buffer, BUF_SIZE - 1)) > 0) { // считываем сообщения клиента
        buffer[bytes_read] = '\0'; 
        std::cout << "Сообщение от клиента: " << buffer << std::endl;
        memset(buffer, 0, BUF_SIZE);
        
        write(new_socket, reply.c_str(), reply.length());
    }
    
    close(new_socket);
    close(server_fd);
}
