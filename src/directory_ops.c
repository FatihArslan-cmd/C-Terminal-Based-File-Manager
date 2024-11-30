#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>  // For creat()
#include <string.h>  // For strcpy, strcmp
#include "file_manager.h"

#ifdef _WIN32
#include <direct.h>  // For _mkdir on Windows
#include <io.h>      // For _unlink on Windows
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

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
                #ifdef _WIN32
                if (_unlink(path) != 0) {  // Use _unlink for Windows
                #else
                if (unlink(path) != 0) {  // Use unlink for Linux
                #endif
                    perror("Failed to delete file");
                    log_operation("delete_folder", path, 0);  // log file deletion failure
                } else {
                    log_operation("delete_folder", path, 1);  // log file deletion success
                }
            }
        }
    }

    closedir(dir);

    #ifdef _WIN32
    if (_rmdir(folder_name) != 0) {  // Use _rmdir for Windows
    #else
    if (rmdir(folder_name) != 0) {  // Use rmdir for Linux
    #endif
        perror("Failed to delete directory");
        log_operation("delete_folder", folder_name, 0);  // log folder deletion failure
    } else {
        printf("Deleted folder: %s\n", folder_name);
        log_operation("delete_folder", folder_name, 1);  // log folder deletion success
    }
}

// Function to list directory contents
void list_directory(const char *path) {
    const char *directory_path = path && strlen(path) > 0 ? path : "."; // Default to current directory if path is NULL or empty

    DIR *dir = opendir(directory_path);
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

        snprintf(full_path, sizeof(full_path), "%s/%s", directory_path, entry->d_name);

        if (stat(full_path, &file_stat) == 0) {
            printf("%s - %ld bytes\n", entry->d_name, file_stat.st_size);
        } else {
            perror("Error getting file info");
            log_operation("list_directory", full_path, 0);  // log failure
        }
    }

    closedir(dir);
    log_operation("list_directory", directory_path, 1);  // log success
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
    #ifdef _WIN32
    // Windows version of mkdir
    if (_mkdir(dirname) != 0) {
    #else
    // Linux version of mkdir with permissions
    if (mkdir(dirname, 0777) == -1) {
    #endif
        perror("Failed to create directory");
        log_operation("create_directory", dirname, 0);  // log failure
        return;
    }

    printf("Directory '%s' created successfully.\n", dirname);
    log_operation("create_directory", dirname, 1);  // log success
}
void delete_file(const char *file_name) {
    if (file_name == NULL || strlen(file_name) == 0) {
        printf("Invalid file name.\n");
        log_operation("delete_file", "Invalid file name", 0);
        return;
    }

    #ifdef _WIN32
    if (_unlink(file_name) != 0) {  // Windows için _unlink
    #else
    if (unlink(file_name) != 0) {  // Linux/macOS için unlink
    #endif
        perror("Failed to delete file");
        log_operation("delete_file", file_name, 0);  // Başarısızlığı logla
    } else {
        printf("Deleted file: %s\n", file_name);
        log_operation("delete_file", file_name, 1);  // Başarıyı logla
    }
}
void print_working_directory() {
    char cwd[1024];  // Buffer to store the current working directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current Working Directory: %s\n", cwd);
        log_operation("print_working_directory", cwd, 1);  // log success
    } else {
        perror("Failed to get current working directory");
        log_operation("print_working_directory", "Failed to get cwd", 0);  // log failure
    }
}