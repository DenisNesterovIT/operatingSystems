#include <stdio.h>
#include <stdlib.h>

int const_tri(int *p, int n){
    if (n == 0)
    {
        return 0;
    }else if (n <= 3){
        return p[n - 1];
    }else{
        for (int i = 3; i < n; i++)
        {
            int temp = p[0] + p[1] + p[2];
            p[0] = p[1];
            p[1] = p[2];
            p[2] = temp;
        }
    }
    return p[2];
}

int main(){
    const int x = 1;
    const int * q = &x;
    int * const p = malloc(3*sizeof(int));
    p[0] = x;
    p[1] = x;
    p[2] = 2*x;
    printf("%p %p %p\n", p, p + 1, p + 2);
    printf("Sixth tribonacci term is equal to %d\n", const_tri(p, 6));
    free(p);



    return 0;
}