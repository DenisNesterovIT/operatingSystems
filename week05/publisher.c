#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    char pipe_path[20];
    for (int i = 1; i <= n; i++) {
        sprintf(pipe_path, "/tmp/ex1/s%d", i);
        mkfifo(pipe_path, 0666);
    }
    for (int i = 1; i <= n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            sprintf(pipe_path, "/tmp/ex1/s%d", i);
            int pipe_fd = open(pipe_path, O_WRONLY);
            char message[100];
            while (fgets(message, sizeof(message), stdin) != NULL) {
                write(pipe_fd, message, sizeof(message));
            }
            close(pipe_fd);
            exit(0);
        }
    }
    return EXIT_SUCCESS;
}
