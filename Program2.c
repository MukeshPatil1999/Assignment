#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

// Function to count files and directories in a directory
void countFilesAndDirectories(const char *path, int *fileCount, int *dirCount) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            struct stat statbuf;
            char childPath[PATH_MAX];
            snprintf(childPath, sizeof(childPath), "%s/%s", path, entry->d_name);

            if (stat(childPath, &statbuf) == 0) {
                if (S_ISDIR(statbuf.st_mode)) {
                    (*dirCount)++;
                    countFilesAndDirectories(childPath, fileCount, dirCount);
                } else if (S_ISREG(statbuf.st_mode)) {
                    (*fileCount)++;
                }
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *rootPath = argv[1];
    int totalFileCount = 0;
    int totalDirCount = 0;

    countFilesAndDirectories(rootPath, &totalFileCount, &totalDirCount);

    printf("Total files: %d\n", totalFileCount);
    printf("Total directories: %d\n", totalDirCount);

    return EXIT_SUCCESS;
}
