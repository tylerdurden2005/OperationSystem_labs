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

int main(){
    int server_fd, new_socket; 
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // создание нового сокета
    
    struct sockaddr_in address; // эта структура содержит интернет адрес
    address.sin_family = AF_INET; // код семейства адресов
    address.sin_port = htons(PORT); // номер порта при помощи htons преобразуется в сетевой порядок байтов
    
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr); // запись айпи адреса в структуру
    connect(server_fd, (struct sockaddr*)&address, sizeof(address)); // становления соединения с сервером
    
    std::string buffer = "Client id = " + std::to_string(getpid());
    char reply[BUF_SIZE];
    memset(reply, 0, BUF_SIZE);
    
    auto start = std::chrono::high_resolution_clock::now(); 
    write(server_fd, buffer.c_str(), buffer.length());  // отправляем серверу
    
    int bytes_read = read(server_fd, reply, BUF_SIZE - 1); // получаем
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Время отклика от pid =  " << getpid() << " составило: "<< duration.count() << " мкс" << std::endl;
    close(server_fd);
}
