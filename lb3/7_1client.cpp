/*Клиент для tcp сервера */

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring> 
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 256

int main(){
    int server_fd, new_socket; 
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // создание нового сокета
    
    struct sockaddr_in address; // эта структура содержит интернет адрес
    address.sin_family = AF_INET; // код семейства адресов
    address.sin_port = htons(PORT); // номер порта при помощи htons преобразуется в сетевой порядок байтов
    
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr); // запись айпи адреса в структуру
    connect(server_fd, (struct sockaddr*)&address, sizeof(address)); // становления соединения с сервером
    
    char buffer[BUF_SIZE];
    while (true) {
    	memset(buffer, 0, BUF_SIZE);
        std::cout << "Введите сообщение (exit для выхода): ";
        fgets(buffer, BUF_SIZE, stdin);
	buffer[strlen(buffer) - 1] = '\0';

        if (strcmp(buffer, "exit") == 0) break;
        write(server_fd, buffer, strlen(buffer));  // отправляем серверу
        memset(buffer, 0, BUF_SIZE);
        int bytes_read = read(server_fd, buffer, BUF_SIZE - 1); // получаем
        if (bytes_read > 0) {
            std::cout << "Ответ от сервера: " << buffer << std::endl;
        }
    }

    close(server_fd);
}
