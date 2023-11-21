#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
    DIR *dir = opendir("/");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    return EXIT_SUCCESS;
}
