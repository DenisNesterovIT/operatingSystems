#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    FILE *pid_file = fopen("/var/run/agent.pid", "r");
    if (pid_file == NULL) {
        printf("Error: No agent found.\n");
        exit(1);
    }
    
    fclose(pid_file);
    printf("Agent found.\n");
    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"} to send to the agent: ");
        char command[10];
        scanf("%s", command);
        int pid = atoi(fopen("/var/run/agent.pid", "r"));
        if (strcmp(command, "read") == 0) {
            kill(pid, SIGUSR1);
        } else if (strcmp(command, "exit") == 0) {
            kill(pid, SIGUSR2);
            break;
        } else if (strcmp(command, "stop") == 0) {
            kill(pid, SIGSTOP);
        } else if (strcmp(command, "continue") == 0) {
            kill(pid, SIGCONT);
        } else {
            printf("Invalid command\n");
        }
    }
    
    return 0;
}
