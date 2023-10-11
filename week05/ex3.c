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

typedef struct prime_request{
    int a, b, count;
} prime_request;

int primes_count(int a, int b) {
    int ret = 0;
    for (int i = a; i < b; i++)
    if (is_prime(i) != 0) (ret)++;
    return ret; 
}

void* prime_counter(void* arg) {
    prime_request* req = (prime_request*) arg;
    int count = (primes_count(req->a, req->b));
    req->count = count;
    return ((void *)count);
}

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int total = 0;
    pthread_t* threads = malloc(m * sizeof(pthread_t));
    prime_request* prime_requests = malloc(m * sizeof(prime_request));
    int subinterval= n / m;
    for (int i = 0; i < m; i++) {
        prime_requests[i].a = i * subinterval;
        prime_requests[i].b = (i == m - 1) ? n : (i + 1) * subinterval;
        pthread_create(&threads[i], NULL, prime_counter, &prime_requests[i]);
    }
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < m; i++)
    {
        total += prime_requests[i].count;
    }
    
    printf("Number of primes is %d\n", total);
    free(threads);
    free(prime_requests);
    return EXIT_SUCCESS;
}
