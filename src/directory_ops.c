#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>  // For creat()
#include <string.h>  // For strcpy, strcmp
#include "file_manager.h"

// Function to delete folder and its contents recursively
void delete_folder(const char *folder_name) {
    DIR *dir = opendir(folder_name);
    if (!dir) {
        perror("Unable to open directory");
        log_operation("delete_folder", "Failed to open directory", 0);  // log failure
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
                delete_folder(path);  // Recursively delete subdirectories
            } else {
                if (unlink(path) != 0) {
                    perror("Failed to delete file");
                    log_operation("delete_folder", path, 0);  // log file deletion failure
                } else {
                    log_operation("delete_folder", path, 1);  // log file deletion success
                }
            }
        }
    }

    closedir(dir);

    if (rmdir(folder_name) != 0) {
        perror("Failed to delete directory");
        log_operation("delete_folder", folder_name, 0);  // log folder deletion failure
    } else {
        printf("Deleted folder: %s\n", folder_name);
        log_operation("delete_folder", folder_name, 1);  // log folder deletion success
    }
}

// Function to list directory contents
void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Unable to open directory");
        log_operation("list_directory", "Failed to open directory", 0);  // log failure
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
            log_operation("list_directory", full_path, 0);  // log failure
        }
    }

    closedir(dir);
    log_operation("list_directory", path, 1);  // log success
}

// Function to create a file
void create_file(const char *filename) {
    int fd = creat(filename, S_IRUSR | S_IWUSR);  // rw-r--r-- permission
    if (fd == -1) {
        perror("Failed to create file");
        log_operation("create_file", filename, 0);  // log failure
        return;
    }

    printf("File '%s' created successfully.\n", filename);
    log_operation("create_file", filename, 1);  // log success

    close(fd);
}

// Function to create a directory
void create_directory(const char *dirname) {
    if (mkdir(dirname) == -1) {  // rwx permissions for the user
        perror("Failed to create directory");
        log_operation("create_directory", dirname, 0);  // log failure
        return;
    }

    printf("Directory '%s' created successfully.\n", dirname);
    log_operation("create_directory", dirname, 1);  // log success
}


