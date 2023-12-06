#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    int status;
    pthread_t threadId;
    int* lockedMutexes;
} Thread;

FILE *file;
int m, n;
pthread_mutex_t* mutexes;
Thread* threads;

int check_deadlock(int L[m + n]){
    int exists[m + n];
    for (int i = 0; i < m + n; i++)
    {
        exists[i] = -1;
    }
    for (int i = 0; i < n + m; i++)
    {
        if (L[i] != -1)
        {
            if (exists[L[i]] != -1)
            {
                return 1;
            }else{
                exists[L[i]] = 1;
            }
            
        }
        
    }
    return 0;
}

void *start_routine(void *arg) {
    int threadId = *(int *)arg;
    printf("Thread %d is created.\n", threadId);    
        for (int i = 0; i < m; i++) {
        if (threads[threadId].lockedMutexes[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
    char string[3];
    fgets(string, 3, file);
    int resource = atoi(string);
    printf("Thread %d tries to lock the mutex %d.\n", threadId, resource);
    if (pthread_mutex_trylock(&mutexes[resource]) == 0) {
        threads[threadId].lockedMutexes[resource] = 1;
        sleep(rand() % 5);
        pthread_mutex_unlock(&mutexes[resource]);
        threads[threadId].lockedMutexes[resource] = 0;
        printf("Thread %d unlocked mutex %d.\n", threadId, resource);
        int unlockedMutexes = 0;
        for (int i = 0; i < n; i++) {
            if (threads[threadId].lockedMutexes[i] == 1) {
                unlockedMutexes = 1;
                break;
            }
        }
        if (unlockedMutexes == 0) {
            pthread_exit(NULL);
        }
    } else {
        int graph[m + n][m + n];
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (threads[i].lockedMutexes[j])
                {
                    graph[i][m + j] = 1;
                }
                
            }
            
        }
        graph[threadId][resource] = 1;
        for(int i; i < m + n; i++){
            int L[m + n];
            for (int i = 0; i < m + n; i++)
            {
                L[i] = -1;
            }
            int count = 0;
            int current = 0;
            int a = 1;
            while (a == 1)
            {
                L[count] = current;
                if (check_deadlock(L) == 1)
                {
                    printf("Deadlock is detected");
                    exit(EXIT_SUCCESS);
                }
                
                int isWay = 0;
                for (int j = 0; j < m + n; j++)
                {
                    if (isWay == 0 && graph[current][j] == 1)
                    {
                        current = j;
                        graph[current][j] = 0;
                        isWay = 1;
                        count++;
                        break;
                    }
                    
                }
                if (isWay == 0)
                {
                    L[count] = -1;
                    count--;
                    current = L[count];
                }
                if (current == 0)
                {
                    a = 0;
                }
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    m = atoi(argv[1]);
    n = atoi(argv[2]);
    file = fopen("input.txt", "r");
    mutexes = malloc(m * sizeof(pthread_mutex_t));
    threads = malloc(n * sizeof(Thread));
    for (int i = 0; i < m; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }
    char string[3];
    while(fgets(string, 3, file) != NULL){
        int thread = atoi(string);
        int *threadId = malloc(sizeof(int));
        *threadId = thread;
        threads[thread].lockedMutexes = malloc(sizeof(int) * m);
        pthread_create(&threads[thread].threadId, NULL, start_routine, (void *)threadId);
    }
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i].threadId, NULL);
    }
    for (int i = 0; i < m; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
    printf("No deadlocks\n");
    for (int i = 0; i < n; i++)
    {
        free(threads[i].lockedMutexes);
    }
    
    free(mutexes);
    free(threads);
    fclose(file);
    return EXIT_SUCCESS;
}