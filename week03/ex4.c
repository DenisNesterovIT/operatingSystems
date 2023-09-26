#include <stdio.h>
#include <stdlib.h>

void* add(const void* a, const void* b) {
    printf("%lu\n", sizeof(b));
    double result = *(double*)a + *(double*)b;
    return &result;
}

void* multiply(const void* a, const void* b) {
    double result = *(double*)a * *(double*)b;
    return &result;
}

void* max(const void* a, const void* b) {
    return (*(double*)a > *(double*)b) ? (void*)a : (void*)b;
}

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
    void* result = initial_value;
    for (int i = 0; i < n; i++) {
        result = opr(result, base + i * size);
    }
    return result;
}

int main() {
    double double_array[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    int int_array[] = {1, 2, 3, 4, 5};

    double initial_double_add = 0.0;
    double* sum_double = (double*)aggregate(double_array, sizeof(double), 5, &initial_double, add);
    printf("Sum of double_array: %lf\n", *sum_double);
    double initial_double_mult = 1.0;
    double* product_double = (double*)aggregate(double_array, sizeof(double), 5, &initial_double_mult, multiply);
    printf("Product of double_array: %lf\n", *product_double);
    double initial_double_max = double_array[0];
    double* max_double = (double*)aggregate(double_array, sizeof(double), 5, &initial_double_max, max);
    printf("Max of double_array: %lf\n", *max_double);
    int initial_int_add = 0;
    int* sum_int = (int*)aggregate(int_array, sizeof(int), 5, &initial_int_add, add);
    printf("Sum of int_array: %d\n", *sum_int);
    int initial_int_mult = 1;
    int* product_int = (int*)aggregate(int_array, sizeof(int), 5, &initial_int_mult, multiply);
    printf("Product of int_array: %d\n", *product_int);
    int initial_int_max = int_array[0];
    int* max_int = (int*)aggregate(int_array, sizeof(int), 5, &initial_int_max, max);
    printf("Max of int_array: %d\n", *max_int);

    return 0;
}