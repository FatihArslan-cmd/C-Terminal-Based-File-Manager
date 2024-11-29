#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>  // For creat()
#include <string.h>  // For strcpy, strcmp
#include "file_manager.h"

// Function to delete a folder and its contents
void delete_folder(const char *folder_name) {
    DIR *dir = opendir(folder_name);
    if (!dir) {
        perror("Unable to open directory");
        return;
    }

    struct dirent *entry;
    char path[1024];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", folder_name, entry->d_name);

        struct stat stat_buf;
        if (stat(path, &stat_buf) == 0) {
            if (S_ISDIR(stat_buf.st_mode)) {
                delete_folder(path);
            } else {
                if (unlink(path) != 0) {
                    perror("Failed to delete file");
                }
            }
        }
    }

    closedir(dir);

    if (rmdir(folder_name) != 0) {
        perror("Failed to delete directory");
    } else {
        printf("Deleted folder: %s\n", folder_name);
    }
}

// Function to list the contents of a directory with file sizes
void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Unable to open directory");
        return;
    }

    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];  

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == 0) {
            printf("%s - %ld bytes\n", entry->d_name, file_stat.st_size);
        } else {
            perror("Error getting file info");
        }
    }
    closedir(dir);
}

// Function to create a new file
void create_file(const char *filename) {
    // Using the `creat()` system call to create the file
    int fd = creat(filename, S_IRUSR | S_IWUSR);  // rw-r--r-- permission
    if (fd == -1) {
        perror("Failed to create file");
        return;
    }

    printf("File '%s' created successfully.\n", filename);

    // Close the file descriptor
    close(fd);
}

// Function to create a new directory
void create_directory(const char *dirname) {
    // Using the `mkdir()` system call to create a directory
    if (mkdir(dirname, S_IRWXU) == -1) {  // rwx permissions for the user
        perror("Failed to create directory");
        return;
    }

    printf("Directory '%s' created successfully.\n", dirname);
}
