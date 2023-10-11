#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 1024

int main() {
    int pipefd[2];
    pid_t pid;
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        close(pipefd[1]); 
        char message[MAX_SIZE];
        int size;
        while ((size = read(pipefd[0], message, MAX_SIZE)) > 0) {
            write(STDOUT_FILENO, message, size);
        }
        close(pipefd[0]);  
        exit(EXIT_SUCCESS);
    } else { 
        close(pipefd[0]); 
        char message[MAX_SIZE];
        printf("Enter a message: ");
        fgets(message, MAX_SIZE, stdin);
        write(pipefd[1], message, strlen(message));
        close(pipefd[1]);  
        wait(NULL); 
        exit(EXIT_SUCCESS);
    }
}
