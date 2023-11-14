#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>

const char *path;

void print_stat_info(const char *path) {
    struct stat st;
    stat(path, &st);
    printf("Mode: %o\n", st.st_mode);
    printf("Inode: %lu\n", st.st_ino);
    printf("Size: %ld bytes\n", st.st_size);
    printf("Stat info for %s:\n", path);
    printf("UID: %u\n", st.st_uid);
    printf("GID: %u\n", st.st_gid);
    printf("Modification Time: %s", ctime(&st.st_mtime));
}

void find_all_hlinks(const char *source) {
    struct stat st;
    lstat(source, &st);
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);
    while ((entry = readdir(dir)) != NULL) {
        struct stat entry_stat;
        char entryPath[PATH_MAX];
        realpath(entry->d_name, entryPath);
        if (entry_stat.st_ino == st.st_ino && S_ISREG(entry_stat.st_mode)) {
            printf("Inode: %lu, Path: %s", entry_stat.st_ino, entryPath);
        }
    }
    closedir(dir);
}

void unlink_all(const char *source) {
    struct stat st;
    lstat(source, &st);
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);
    while ((entry = readdir(dir)) != NULL) {
        struct stat entry_stat;
        char entry_path[PATH_MAX];
        snprintf(entry_path, PATH_MAX, "%s/%s", ".", entry->d_name);
        lstat(entry_path, &entry_stat);
        if (entry_stat.st_ino == st.st_ino && S_ISREG(entry_stat.st_mode) && strcmp(entry_path, source) != 0) {
            unlink(entry_path);
        }
    }
    printf("Last hard link is %s\n", source);
    closedir(dir);
}

create_sym_link(const char* source, const char* link){
    symlink(source, link);
}

int main(int argc, char *argv[]) {
    path = argv[1];
    FILE *file = fopen(strcat(path,"myfile1.txt"), "w");
    fprintf(file, "Hello world.\n");
    fclose(file);
    link("myfile1.txt", "myfile11.txt");
    link("myfile1.txt", "myfile12.txt");
    find_all_hlinks("myfile1.txt");
    rename("myfile1.txt", "/tmp/myfile1.txt");
    file = fopen(strcat(path,"myfile11.txt"), "a");
    fprintf(file, "Modified content.\n");
    fclose(file);
    create_sym_link("/tmp/myfile1.txt", "myfile13.txt");
    file = fopen("/tmp/myfile1.txt", "a");
    fprintf(file, "Modified content in /tmp/myfile1.txt.\n");
    fclose(file);
    unlink_all("myfile11.txt");
    print_stat_info("myfile11.txt");
    return EXIT_SUCCESS;
}
