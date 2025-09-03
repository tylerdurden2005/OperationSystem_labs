#include <iostream>

int main() {
    std::cout << "Введите полный путь до файла: ";
    std::string fileName;
    std::cin >> fileName;
    FILE* file = fopen(fileName.c_str(), "r");
    if (file == nullptr){
        std::cerr << "Не удалось открыть файл!\n";
        return 1;
    }
    char text[256];
    while (fgets(text, 256, file)){
        std::cout << text;
    }
    fclose(file);
    return 0;
}
