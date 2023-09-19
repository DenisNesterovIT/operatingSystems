#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <float.h>

int main(){
    int integer = INT_MAX;
    unsigned short unShort = USHRT_MAX;
    signed long sinLong = LONG_MAX;
    float fl = FLT_MAX;
    double db = DBL_MAX;
    printf("Integer size: %lu, value: %d\n", sizeof(integer), integer);
    printf("Unsigned short integer size: %lu, value: %d\n", sizeof(unShort), unShort);
    printf("Signed long integer size: %lu, value: %ld\n", sizeof(sinLong), sinLong);
    printf("Float size: %lu, value: %f\n", sizeof(fl), fl);
    printf("Double size: %lu, value: %f\n", sizeof(db), db);
    return 0;
}