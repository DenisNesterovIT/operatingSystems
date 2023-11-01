#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

struct PTE *pageTable;
int P;

void display_pageTable(){
    for (int i = 0; i < P; i++)
    {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, pageTable[i].valid, pageTable[i].frame, pageTable[i].dirty, pageTable[i].referenced);
    }
}


int main(int argc, char *argv[]) {
    P = atoi(&argv[1]);
    pid_t pagerPid = atoi(&argv[strlen(*argv)]);
    int file = open("/tmp/ex2/pagetable", O_RDWR);
    pageTable = (struct PTE *)mmap(NULL, sizeof(struct PTE) * P, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    printf("-------------------------\n");
    printf("Initialized page table\n");
    display_pageTable();
    printf("-------------------------\n");
    for (int i = 2; i < strlen(*argv) - 1; i++)
    {
        switch (argv[i][0])
        {
        case 'W':
            printf("Write Request for page %d\n", atoi(&argv[i][1]));
            break;
        case 'R':
            printf("Read Request for page %d\n", atoi(&argv[i][1]));
            break;
        default:
            break;
        }
        if (!pageTable[atoi(&argv[i][1])].valid) {
            printf("It is not a valid page --> page fault\n");
            printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
            pageTable[atoi(&argv[i][1])].referenced = getpid();
            kill(pagerPid, SIGUSR1);
            pause();
            printf("MMU resumed by SIGCONT signal from pager\n");
        } else {
            printf("It is a valid page\n");
            if (argv[i][0] == 'W')
            {
                pageTable[atoi(&argv[i][1])].dirty = true;
                printf("It is a write request then set the dirty field\n");
            }
        }
        printf("Page table\n");
        display_pageTable();
        printf("-------------------------\n");
    }
    close(file);
    kill(pagerPid, SIGUSR1);
    return EXIT_SUCCESS;
}
