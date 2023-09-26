#include <stdio.h>
#include <stdlib.h>

void* add_int(const void* a, const void* b) {
    int result = *(int*)a + *(int*)b;
    return &result;
}

void* multiply_int(const void* a, const void* b) {
    int result = *(int*)a * *(int*)b;
    return &result;
}

void* maxInt(const void* a, const void* b) {
    return (*(int*)a > *(int*)b) ? (void*)a : (void*)b;
}

void* add_double(const void* a, const void* b) {
    double result = *(double*)a + *(double*)b;
    return &result;
}

void* multiply_double(const void* a, const void* b) {
    double result = *(double*)a * *(double*)b;
    return &result;
}

void* maxDouble(const void* a, const void* b) {
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
    double double_array[] = {2.0, 4.0, 6.0, 8.0, 10.0};
    int int_array[] = {2, 4, 6, 8, 10};
    double initial_double_add = 0.0;
    double* sum_double = (double*)aggregate(double_array, sizeof(double), 5, &initial_double_add, add_double);
    printf("Sum of double: %lf\n", *sum_double);
    double initial_double_mult = 1.0;
    double* product_double = (double*)aggregate(double_array, sizeof(double), 5, &initial_double_mult, multiply_double);
    printf("Product of double: %lf\n", *product_double);
    double initial_double_max = double_array[0];
    double* max_double = (double*)aggregate(double_array, sizeof(double), 5, &initial_double_max, maxDouble);
    printf("Max of double: %lf\n", *max_double);
    int initial_int_add = 0;
    int* sum_int = (int*)aggregate(int_array, sizeof(int), 5, &initial_int_add, add_int);
    printf("Sum of int: %d\n", *sum_int);
    int initial_int_mult = 1;
    int* product_int = (int*)aggregate(int_array, sizeof(int), 5, &initial_int_mult, multiply_int);
    printf("Product of int: %d\n", *product_int);
    int initial_int_max = int_array[0];
    int* max_int = (int*)aggregate(int_array, sizeof(int), 5, &initial_int_max, maxInt);
    printf("Max of int: %d\n", *max_int);
    return 0;
}