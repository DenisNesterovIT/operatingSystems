#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char command[100];
    char *arguments[10];
    int background = 0;
    while (1) {
        printf("Enter a command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';
        if (command[strlen(command) - 1] == '&') {
            background = 1;
            command[strlen(command) - 1] = '\0';
        } else {
            background = 0;
        }
        char *token = strtok(command, " ");
        int i = 0;
        while (token != NULL && i < 10 - 1) {
            arguments[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        arguments[i] = NULL;
        pid_t pid = fork();
        if (pid == 0) {
            execvp(arguments[0], arguments);
            printf("Failed to execute command.\n");
            exit(1);
        } else if (pid < 0) {
            printf("Failed to create child process.\n");
            return 1;
        }
        if (!background) {
            wait(NULL);
        }
    }

    return 0;
}
