#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

int k = 0; 
int c = 0;
int n = 0;

int get_number_to_check() {
    int ret = k; 
    if (k != n) k++; 
    return ret;
}

void increment_primes() {
    c++; 
}

void* check_primes(void* arg) {
    int end = *((int*)arg);
    pthread_mutex_lock(&global_lock);
    while (1)
    {
        int num = get_number_to_check();
        if (num < end)
        {
            if (is_prime(num)) {
                increment_primes();
            }
        }
        else{
            pthread_mutex_unlock(&global_lock);
            return NULL;
        }
    }
    
}

int main(int argc, char* argv[]) {
    n = atoi(argv[1]);
    int m = atoi(argv[2]);
    pthread_mutex_init(&global_lock, NULL);
    pthread_t threads[m];
    int subinterval = n / m;
    for (int i = 0; i < m; i++) {
        int end = (i + 1) * subinterval;
        pthread_create(&threads[i], NULL, check_primes, &end);
    }
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&global_lock);
    printf("Number of primes is %d\n", c);
    return EXIT_SUCCESS;
}
