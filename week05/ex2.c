#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 256

typedef struct Thread {
    pthread_t id;
    int i;
    char message[MAX_SIZE];
}Thread;

void* threadFunction(void* arg) {
    Thread* thread = (Thread*) arg;
    printf("Thread %d is created\n", thread->i);
    printf("Thread id: %u, its message: %s\n", thread->id, thread->message);
    pthread_exit(NULL);
}

int main() {
    int n;
    printf("Enter number of threads: ");
    scanf("%d", &n);
    Thread threads[n];
    for (int i = 0; i < n; i++) {
        threads[i].i = i;
        sprintf(threads[i].message, "Hello from thread %d", threads[i].i);
        pthread_create(&threads[i].id, NULL, threadFunction, &threads[i]);
        pthread_join(threads[i].id, NULL);
    }

    exit(EXIT_SUCCESS);
}
