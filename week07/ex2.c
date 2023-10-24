#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#define FILE_SIZE 1024*1024*500
#define LINE_SIZE 1024


int main() {
    FILE * text = fopen("text.txt", "w");
    int random = open("/dev/random", O_RDONLY);
    int fileSizeCount = 0;
    while(fileSizeCount < FILE_SIZE){
        char* c;
        read(random, c, 1);
        if (isprint(*c))
        {
            fprintf(text, "%c", *c);
            fileSizeCount++;
            if (fileSizeCount % LINE_SIZE == 0)
            {
                fprintf(text, "%s", "\n");
            }
        }
    }
    close(random);
    fclose(text);
    int file = open("text.txt", O_RDWR);
    long sz = sysconf(_SC_PAGESIZE);
    struct stat file_info;
    fstat(file, &file_info);
    int fileSize = (int)file_info.st_size;
    int chunkSize = 1024*sz;
    char *data = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    int capitalLetters = 0;
    for (size_t i = 0; i < fileSize; i++) {
        if (isupper(data[i])) {
            capitalLetters++;
            data[i] = tolower(data[i]);
        }
    }
    printf("Number of capital letters is %d\n", capitalLetters);
    munmap(data, fileSize);
    close(file);
    return 0;
}
