#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring> 
#include <arpa/inet.h>
#include <chrono>

#define PORT 8080
#define BUF_SIZE 256

int main() {
    int server_fd;
    server_fd = socket(AF_INET, SOCK_DGRAM, 0); // создание нового сокета
    
    struct sockaddr_in address; // эта структура содержит интернет адрес
    address.sin_family = AF_INET; // код семейства адресов
    address.sin_port = htons(PORT); // номер порта при помощи htons преобразуется в сетевой порядок байтов
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); // локальный IP
    
    struct sockaddr_in from_address;
    socklen_t addrlen = sizeof(from_address);
    std::string buffer = "Client id = " + std::to_string(getpid());
    char reply[BUF_SIZE];
    auto start = std::chrono::high_resolution_clock::now(); 
    
    sendto(server_fd, buffer.c_str(), buffer.length(), 0, (const struct sockaddr *)&address, sizeof(address)); // Отправка сообщения на сервер
    memset(reply, 0, BUF_SIZE);
    int n = recvfrom(server_fd, reply, BUF_SIZE, 0, (struct sockaddr *)&from_address, &addrlen); // получение сообщения от сервера
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Время отклика от pid =  " << getpid() << " составило: "<< duration.count() << " мкс" << std::endl;
    /*reply[n] = '\0'; 
    std::cout << "Ответ от сервера: " << reply << std::endl; */

    close(server_fd);
    return 0;
}
