#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include<sys/wait.h>

int main(){
    clock_t main = clock();
    pid_t child_one, child_two;
    child_one = fork();
    clock_t child_one_t = clock();
    clock_t child_two_t;
    if (child_one == 0) {
        printf("Child one id - %d, parent of child one id - %d\n", getpid(), getppid());
    } else {
        printf("Parent id - %d, parent of parent id - %d\n", getpid(), getppid());
        child_two = fork();
        child_two_t = clock();
        wait(NULL);
        if (child_two == 0) {
            printf("Child two id - %d, parent of child two id - %d\n", getpid(), getppid());
        } else {
            wait(NULL);
            printf("Execution time of main is %lu, of child one is %lu, of child two is %lu\n", clock() - main, clock() - child_one_t, clock() - child_two_t);
        }
    }
}


