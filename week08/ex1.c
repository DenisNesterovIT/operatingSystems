#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

int main() {
    int randomFile = open("/dev/random", O_RDONLY);
    char password[14];
    int position = 5;
    password[0] = 'p';
    password[1] = 'a';
    password[2] = 's';
    password[3] = 's';
    password[4] = ':';
    while (position < 13)
    {
        char character;
        read(randomFile, &character, 1);
        if (isprint(character))
        {
            password[position] = character;
            position++;
        }
    }
    close(randomFile);
    FILE * pidFile = fopen("/tmp/ex1.pid", "w");
    if (pidFile != NULL) {
        fprintf(pidFile, "%d", getpid());
        fclose(pidFile);
        printf("%d\n", getpid());
    }
    char *passwordMemory = (char *)mmap(NULL, 13, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (int i = 0; i < 13; i++)
    {
        passwordMemory[i] = password[i];
    }
    while (1) {
        sleep(1); 
    }
    return EXIT_SUCCESS;
}
