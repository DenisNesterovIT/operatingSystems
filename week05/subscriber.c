#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int id = atoi(argv[1]);
    char pipe_path[20];
    sprintf(pipe_path, "/tmp/ex1/s%d", id);
    int pipe_fd = open(pipe_path, O_RDONLY);
    char message[100];
    while (read(pipe_fd, message, sizeof(message)) > 0) {
        printf("Subscriber %d: %s", id, message);
    }
    close(pipe_fd);
    return EXIT_SUCCESS;
}
