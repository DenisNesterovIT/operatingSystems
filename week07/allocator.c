#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#define MAX_HEAP 10000000

int memory[MAX_HEAP];

void allocate_first_fit(unsigned int adrs, int size){
    for (int i = 0; i < MAX_HEAP; i++) {
        if (memory[i] == 0) {
            int j;
            for (j = i; j < i + size; j++) {
                if (j >= MAX_HEAP || memory[j] != 0) {
                    break;
                }
            }
            if (j == i + size) {
                for (j = i; j < i + size; j++) {
                    memory[j] = adrs;
                }
                return;
            }
        }
    }
}

void allocate_best_fit(unsigned int adrs, int size){
    int minSize = INT_MAX;
    int index = -1;
    for (int i = 0; i < MAX_HEAP; i++) {
        if (memory[i] == 0) {
            int blockSize = 0;
            while (i < MAX_HEAP && memory[i] == 0) {
                blockSize++;
                i++;
            }
            if (blockSize >= size && blockSize < minSize) {
                minSize = blockSize;
                index = i - blockSize;
            }
        }
    }
    if (index != -1) {
        for (int i = index; i < index + size; i++) {
            memory[i] = adrs;
        }
    }
}
void allocate_worst_fit(unsigned int adrs, int size){
    int maxSize = 0;
    int index = -1;
    for (int i = 0; i < MAX_HEAP; i++) {
        if (memory[i] == 0) {
            int blockSize = 0;
            while (i < MAX_HEAP && memory[i] == 0) {
                blockSize++;
                i++;
            }
            if (blockSize >= size && blockSize > maxSize) {
                maxSize = blockSize;
                index = i - blockSize;
            }
        }
    }
    if (index != -1) {
        for (int i = index; i < index + size; i++) {
            memory[i] = adrs;
        }
    }
}

void clear(unsigned int adrs){
    int isFound = 0;
    for (int i = 0; i < MAX_HEAP; i++)
    {
        if (memory[i] == adrs)
        {
            memory[i] = 0;
            isFound = 1;
        }else{
            if (isFound)
            {
                return;
            }
        }
    }
}

int main() {
    clock_t time = clock();
    int numberOfQueries = 0;
    FILE * file = fopen("queries.txt", "r");
    while (!feof(file))
    {
        numberOfQueries++;
        char command[8];
        unsigned int adrs;
        int size;
        fscanf(file ,"%s", command);
        if (strcmp(command,"allocate") == 0){
            fscanf(file, "%d", &adrs);
            fscanf(file, "%d", &size);
            allocate_worst_fit(adrs, size);
        }else if (strcmp(command, "clear") == 0){
            fscanf(file, "%d", &adrs);
            clear(adrs);
        }else if (strcmp(command, "end") == 0)
        {
            fclose(file);
            printf("Throughput is - %f\n", (double)(numberOfQueries - 1)/((double) clock() - time));
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Error\n");
            exit(EXIT_FAILURE);
        }
    }
    
    return EXIT_SUCCESS;
}
