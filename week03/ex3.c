#include <stdio.h>
#include <string.h>

struct Directory;
struct File;

typedef struct File
{
    int id;
    char name[63];
    int size;
    char data[1024];
    struct Directory *directory;
} File;

typedef struct Directory
{
    int nf;
    int nd;
    File files[256];
    struct Directory *sub_dirs[63];
    char path[2048];
    char name[63];
} Directory;

void overwrite_to_file(File* file, const char* str){
    strcpy(file->data, str);
}

void append_to_file( File* file, const char* str){
    strcat(file->data, str);
}

void printp_file( File* file){
    char temp_path[2048]; 
    strcpy(temp_path, file->directory->path);
    strcat(temp_path, "/");
    strcat(temp_path, file->name);
    printf("%s\n", temp_path);
}

void add_file(struct File* file, struct Directory* dir){
    dir->files[dir->nf] = *file;
    dir->nf++;
    file->directory = dir;
}

void add_dir(Directory *dir1, Directory *dir2){
    if (dir1 && dir2){
        dir2->sub_dirs[dir2->nd] = dir1;
        dir2->nd++;
        char temp_path[2048];
        if (strcmp(dir2->path, "/")){
            strcpy(temp_path, dir2->path);
            strcat(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
        else{
            strcpy(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
    }
}

int main(){
    Directory root;
    strcpy(root.name, "/");
    root.nd = 2;
    strcpy(root.path, "/");
    Directory bin;
    Directory home;
    strcpy(bin.name, "bin");
    strcpy(home.name, "home");
    add_dir(&bin, &root);
    add_dir(&home, &root);
    File bash;
    bash.id = 1;
    strcpy(bash.name, "bash");
    add_file(&bash, &bin);
    File file1;
    File file2;
    file1.id = 2;
    strcpy(file1.name, "ex3_1.c");
    overwrite_to_file(&file1, "int printf(const char * format, ...);");
    file1.size = strlen(file1.data);
    file2.id = 3;
    strcpy(file2.name, "ex3_2.c");
    overwrite_to_file(&file2, "//This is a comment in C language");
    file2.size = strlen(file2.data);
    add_file(&file1, &home);
    add_file(&file2, &home);
    overwrite_to_file(&bash, "Bourne Again Shell!!");
    bash.size = strlen(bash.data);
    append_to_file(&file1, "int main(){printf(”Hello World!”)}");
    printp_file(&bash);
    printp_file(&file1);
    printp_file(&file2);
    return 0;
}