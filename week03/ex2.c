#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct Point
{
    float x;
    float y;
};

float distance( struct Point p1, struct Point p2){
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

float area(struct Point a, struct Point b, struct Point c){
    return 0.5*abs(a.x*b.y - a.y*b.x + b.x*c.y - b.y*c.x + a.y*c.x - a.x*c.y);
}

struct Point A;
struct Point B;
struct Point C;

int main(){
    A.x = 2.5;
    A.y = 6;
    B.x = 1;
    B.y = 2.2;
    C.x = 10;
    C.y = 6;
    printf("Distance between A and B is %f\n", distance(A, B));
    printf("Area of triangle is %f\n", area(A,B,C));
    return 0;
}


