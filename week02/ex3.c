#include <stdio.h>
#include <math.h>

int convert(long long int x, short s, short t){
    int result[200];
    if (s < 2 || s > 10 || t < 2 || t > 10)
    {
        printf("cannot convert!\n");
        return 0;
    }
    if (s > t){
        int j = 0;
        while (x/t > 0){
            int y = x % t;
            x = x / t;
            result[j] = y;
            j++;
        }
        result[j] = x;
        while (j >= 0){
            printf("%d", result[j]);
            j--;
        }
        printf("\n");
        return 0;
    }else if(s < t){
        long int result = 0;
        int i = 0;
        while (x/10 > 0){
            result += (x % 10)*pow(s, i);
            x = x/10;
            i++;
        }
        result += x * pow(s, i);
        printf("%ld\n", result);
        return 0;
    }else{
        printf("%lld\n", x);
    }
    return 0;
}

int main() {
    long long int x;
    short s;
    short t;
    printf("Enter a number: ");
    scanf("%lld", &x);
    printf("Enter the specified source number system: ");
    scanf("%hd", &s);
    printf("Enter the specified target number system: ");
    scanf("%hd", &t);
    convert(x, s, t);
    return 0;
}