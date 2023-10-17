#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigusr1_handler(int signum) {
    FILE *file = fopen("text.txt", "r");
    if (file == NULL) {
        exit(1);
    }
    char character;
    while ((character = fgetc(file)) != EOF) {
        printf("%c", character);
    }
    printf("\n");
    fclose(file);
}

void sigusr2_handler(int signum) {
    printf("Process terminating...\n");
    exit(0);
}

int main() {
    FILE *pid_file = fopen("/var/run/agent.pid", "w");
    if (pid_file == NULL) {
        exit(1);
    }
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);
    FILE *file = fopen("text.txt", "r");
    if (file == NULL) {
        exit(1);
    }
    char character;
    while ((character = fgetc(file)) != EOF) {
        printf("%c", character);
    }
    printf("\n");
    fclose(file);
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    while (1) {
        sleep(1);
    }
    return 0;
}
