#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <vector>
int main(int argc, char* argv[]){
    int status;
    int pid = fork();
    if (pid == -1){
        std::cout << "Ошибка: Дочерний процесс не создался!\n";
        exit(EXIT_FAILURE);
    }
    else if (pid==0){
    	char *vector_args[] = {(char*)"2_3son", (char*)"arg1", (char*)"arg2", nullptr};
    	char* envp[] = {(char*)"CUSTOM_VAR1=HELLO", (char*)"CUSTOM_VAR2=WORLD!", (char*)"CUSTOM_VAR3=FROM NIKITA!",(char*)"LANGUAGE=RUSSIAN",nullptr};
    	std::string path = getenv("PATH");
    	setenv("PATH", (path + ":/home/nikita/task2_3").c_str(), 1);
    	switch ( (int)argv[1][0] ) {
            case (int)'1':
            	std::cout << "Был выбран execl.\n";
                execl("2_3son", "2_3son", "arg1", "arg2", nullptr);
                break;
            case (int)'2':
            	std::cout << "Был выбран execv.\n";
                execv("2_3son", vector_args); 
                break;
            case (int)'3':
            	std::cout << "Был выбран execle.\n";
                execle("2_3son", "2_3son", "arg1", "arg2", nullptr, envp);
                break;
            case (int)'4':
            	std::cout << "Был выбран execve.\n";
                execve("2_3son", vector_args, envp); 
                break;
            case (int)'5':
            	std::cout << "Был выбран execlp.\n";
                execlp("2_3son", "2_3son", "arg1", "arg2", nullptr); 
                break;
            case (int)'6':
            	std::cout << "Был выбран execvp.\n";
                execvp("2_3son", vector_args);  
                break;
            case (int)'7':
            	std::cout << "Был выбран execvpe.\n";
                execvpe("2_3son", vector_args, envp);  
                break;
            default:
            	std::cout << "Ошибка!\n";
            	exit(EXIT_FAILURE);
        }
    }
    std::cout << "Родитель: pid="<<getpid()<<" ppid="<<getppid() << "\n";
    wait(&status);
    std::cout << "\nДочерний процесс завершился со статусом: " << status << "\n";
    return 0;
}
