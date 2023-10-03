#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define VECTOR_SIZE 120

int dot_product(int *u, int *v, int start, int end) {
    int result = 0;
    for (int i = start; i < end; i++) {
        result += u[i] * v[i];
    }
    return result;
}

int main() {
    int u[VECTOR_SIZE];
    int v[VECTOR_SIZE];
    FILE * file = fopen("temp.txt", "r+");
    // Set random values to the vectors
    for (int i = 0; i < VECTOR_SIZE; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int components_per_process = VECTOR_SIZE / n;

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            int start = i * components_per_process;
            int end = start + components_per_process;
            int result = dot_product(u, v, start, end);
            char string[10];
            sprintf(string, "%d", result);
            fputs(string, file);
            fputs("\n", file);
            exit(0);
        } else if (pid < 0) {
            printf("Failed to create child process.\n");
            return 1;
        } else {
            wait(NULL);
        }
    }
    fclose(file);
    FILE * rfile = fopen("temp.txt", "r");
    int result = 0;
    for (int i = 0; i < n; i++)
    {
        char line[10];
        fscanf(file,"%s[^\n]", line);
        result += atoi(line);
    }
    printf("Dot product is: %d\n", result);
    return 0;
}
