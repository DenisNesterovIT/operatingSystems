#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/inotify.h>

const char *path;

void print_stat_info(const char *path) {
    struct stat st;
    stat(path, &st);
    printf("Mode: %o\n", st.st_mode);
    printf("Inode: %llu\n", st.st_ino);
    printf("Size: %lld bytes\n", st.st_size);
    printf("Stat info for %s:\n", path);
    printf("UID: %u\n", st.st_uid);
    printf("GID: %u\n", st.st_gid);
    printf("Modification Time: %s", ctime(&st.st_mtime));
}

void signal_handler(int signum) {
    printf("Stat info:\n");
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            print_stat_info(entry->d_name);
        }
    }
    closedir(dir);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    path = argv[1];
    int inotifyFd = inotify_init();
    inotify_add_watch(inotifyFd, path, IN_ALL_EVENTS);
    struct dirent *entry;
    DIR * dir = opendir(path);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            print_stat_info(entry->d_name);
        }
    }
    closedir(dir);
    signal(SIGINT, signal_handler);
    char buf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    printf("Monitoring directory: %s\n", path);
    while (1) {
        ssize_t len = read(inotifyFd, buf, sizeof(buf));
        for (char *ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len) {
            event = (const struct inotify_event *) ptr;
            if (event->mask & IN_ACCESS) {
                printf("%s is accessed\n", event->name);
            } else if (event->mask & IN_CREATE) {
                printf("%s is created\n", event->name);
            } else if (event->mask & IN_DELETE) {
                printf("%s is deleted\n", event->name);
            } else if (event->mask & IN_MODIFY) {
                printf("%s is modified\n", event->name);
            } else if (event->mask & IN_OPEN) {
                printf("%s is opened\n", event->name);
            } else if (event->mask & IN_ATTRIB) {
                printf("%s metadata changed\n", event->name);
            }
            char item_path[PATH_MAX];
            snprintf(item_path, PATH_MAX, "%s/%s", path, event->name);
            print_stat_info(item_path);
        }
    }
    close(inotifyFd);
    return EXIT_SUCCESS;
}