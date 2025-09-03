#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring> 

#define PORT 8080
#define BUF_SIZE 256

int main(){
    int server_fd; 
    server_fd = socket(AF_INET, SOCK_DGRAM, 0); // создание нового сокета
    
    struct sockaddr_in address; // эта структура содержит интернет адрес
    address.sin_family = AF_INET; // код семейства адресов
    address.sin_addr.s_addr = INADDR_ANY; // ip адрес хоста (ip  компьютера) - константа INADDR_ANY, которая получает этот адрес
    address.sin_port = htons(PORT); // номер порта при помощи htons преобразуется в сетевой порядок байтов
    
    bind(server_fd, (struct sockaddr*)&address, sizeof(address)); // привязывает сокет к адресу текущего хоста и номеру порта
    
    struct sockaddr_in cli_address;
    socklen_t addrlen = sizeof(cli_address);
    
    char buffer[BUF_SIZE];
    std::string reply = "Сервер получил сообщение!";
    while (true){
    	memset(buffer, 0, BUF_SIZE);
    	int n = recvfrom(server_fd, buffer, BUF_SIZE, 0, (struct sockaddr *)&cli_address, &addrlen); // получение сообщения от клиента
    	buffer[n] = '\0'; 
    	std::cout << "Сообщение от клиента: " << buffer << std::endl; 
    
    	sendto(server_fd, reply.c_str(), reply.length(), 0, (struct sockaddr *)&cli_address, addrlen); // ответ сервера
    }
    
    close(server_fd);
}
