#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

void memory_work() {
    int size = 10 * 1024 * 1024;
    void *ptr = malloc(size);
    memset(ptr, 0, size);
}

int main() {
    struct rusage usage;
    for (int i = 0; i < 10; i++) {
        memory_work();
        getrusage(RUSAGE_SELF, &usage);
        printf("Memory Usage is %ld\n", usage.ru_maxrss / 1024);
        sleep(1);
    }
    return EXIT_SUCCESS;
}
