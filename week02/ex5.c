#include <stdio.h>

long int tribonacci(int n){
    long int array[n + 1];
    switch (n)
    {
    case 0:
        return 0;
        break;
    case 1:
        return 1;
        break;
    case 2:
        return 1;
        break;
    default:
        array[0] = 0;
        array[1] = 1;
        array[2] = 1;
        for (int i = 3; i < n + 1; i++)
        {
            array[i] = array[i - 1] + array[i - 2] + array[i - 3];
        }
        break;
    }
    return array[n];
}

int main(){
    printf("Tribonacci with argument 4: %ld\n", tribonacci(4));
    printf("Tribonacci with argument 36: %ld\n", tribonacci(36));
    return 0;
}