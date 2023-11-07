#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <signal.h>

#define STRING_SIZE 8

struct PTE{
    // The page is in the physical memory (RAM) 
    bool valid;
    // The frame number of the page in the RAM 
    int frame;
    // The page should be written to disk
    bool dirty;
    // The page is referenced/requested
    int referenced;
    int counter;
};

typedef struct{
    char string[STRING_SIZE + 1];
} String;

struct PTE *pageTable;
int P = 0;
int F = 0;
int totalAccess = 0;
int freeFrames;
String* RAM;
String* disk;
char *replacementAlgoithm;
int* counter;


void display_RAM(){
    printf("RAM array\n");
    for (int i = 0; i < F; i++)
    {
        printf("Frame %d ---> %s\n", i, RAM[i].string);
    }
}

void display_disk(){
    printf("Disk array\n");
    for (int i = 0; i < P; i++)
    {
        printf("Page %d ---> %s\n", i, disk[i].string);
    }
}
void display_pageTable(){
    for (int i = 0; i < P; i++)
    {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, pageTable[i].valid, pageTable[i].frame, pageTable[i].dirty, pageTable[i].referenced);
    }
}

void disk_to_RAM(int i, int j){
    strcpy(RAM[j].string, disk[i].string);
}

void RAM_to_disk(int i, int j){
    strcpy(disk[i].string, RAM[j].string);
}

// Random page replacement
int random(){
    return pageTable[rand() % F].frame;
}

// NFU page replacement
int nfu(){
    int minCount = counter[0];
    int index = 0;
    for (int i = 1; i < F; i++) {
        if (counter[i] < minCount) {
            minCount= counter[i];
            index = i;
        }
    }
    return index;
}

// Aging page replacement
int aging(){
    int min_counter = 255;
    int min_page = -1;
    for (int i = 0; i < P; i++) {
        if (pageTable[i].valid) {
            pageTable[i].counter = (pageTable[i].counter >> 1) | (pageTable[i].referenced << 7);
            pageTable[i].referenced = 0; 
            if (pageTable[i].counter < min_counter) {
                min_counter = pageTable[i].counter;
                min_page = i;
            }
        }
    }
    return min_page;
}

void signal_handler(int signum){
    printf("-------------------------\n");
    printf("A disk access request from MMU Process (pid=%d)\n", getpid());
    for (int i = 0; i < P; i++)
    {
        printf("Page %d is referenced\n", i);
        if (pageTable[i].referenced != 0)
        {
            if (freeFrames > 0)
            {
                counter[i]++;
                int freeFrame = F - freeFrames;
                disk_to_RAM(i, freeFrame);
                freeFrames--;
                pageTable[i].valid = true;
                pageTable[i].frame = freeFrame;
                printf("We can allocate it to free frame %d\n", freeFrame);
                printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, freeFrame);
            }else{
                printf("We do not have free frames in RAM\n");
                int victim = rand() % F;
                printf("Chose a random victim page %d\n", victim);
                printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, pageTable[victim].frame);
                if (pageTable[victim].dirty)
                {
                    RAM_to_disk(victim, pageTable[victim].frame);
                    totalAccess++;
                }
                pageTable[victim].valid = false;
                pageTable[victim].dirty = false;
                pageTable[victim].referenced = 0;
                disk_to_RAM(i, pageTable[victim].frame);
                pageTable[i].frame = pageTable[victim].frame;
                pageTable[i].valid = true;
                totalAccess++;
            }
            display_RAM();
            printf("disk accesses is %d so far\n", totalAccess);
            pageTable[i].dirty = 0;
            pageTable[i].referenced = 0;
            kill(0, SIGCONT);
            printf("Resume MMU process\n");
            printf("-------------------------\n");
            return;
            
        }
    }
    printf("%d disk accesses in total\n", totalAccess);
    printf("Pager is terminated\n");
    return;
}

int main(int argc, char* argv[]){
    F =atoi(argv[2]);
    P = atoi(argv[1]);
    replacementAlgoithm = argv[3];
    counter = malloc(sizeof(int) * F);
    printf("You choose %s replacement algorithm\n", replacementAlgoithm);
    freeFrames = F;
    RAM = (String *) malloc(sizeof(String) * F);
    disk = (String *) malloc(sizeof(String) * P);
    int randomFile = open("/dev/random", O_RDONLY);
    for (int i = 0; i < P; i++)
    {
        int position = 0;
        char newString[STRING_SIZE + 1];
        while (position < STRING_SIZE)
        {
            char character;
            read(randomFile, &character, 1);
            if (isprint(character))
            {
                newString[position] = character;
                position++;
            }
        }
        newString[position] = '\0';
        strcpy(disk[i].string, newString);
    }
    int pageTableSize = sizeof(struct PTE) * P;
    mkdir("/tmp/ex2", 0700);
    int file = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, 0666);
    ftruncate(file, pageTableSize);
    pageTable = (struct PTE *)mmap(NULL, pageTableSize, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    for (int i = 0; i < P; i++) {
        pageTable[i].valid = false;
        pageTable[i].dirty = false;
        pageTable[i].frame = -1;
        pageTable[i].referenced = 0;

    }
    printf("-------------------------\n");
    printf("Initialized page table\n");
    display_pageTable();
    printf("-------------------------\n");
    printf("Initialized RAM\n");
    display_RAM();
    printf("-------------------------\n");
    printf("Initialized disk\n");
    display_disk();
    signal(SIGUSR1, signal_handler);
    printf("Pager initialized. PID: %d\n", getpid());
    while(1){
        pause();
    }
    close(file);
    munmap(pageTable, pageTableSize);
    free(RAM);
    free(disk);
    free(counter);
    return EXIT_SUCCESS;
}