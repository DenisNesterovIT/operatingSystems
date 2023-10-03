#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc){
    for (int i = 0; i < argc; i++)
    {
        pid_t proc = fork();
        sleep(5);
    }
}