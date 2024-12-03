#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "file_manager.h"
#include "file_manager_logo.h"

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

// Helper function to check if a path exists
static int path_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

// Helper function to check if a path is a directory
static int is_directory(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

// Helper function to safely concatenate paths
static int safe_path_concat(char *dest, size_t dest_size, const char *dir, const char *file) {
    if (snprintf(dest, dest_size, "%s/%s", dir, file) >= dest_size) {
        return 0; // Path too long
    }
    return 1;
}

// Function to delete folder and its contents recursively
void delete_folder(const char *folder_name) {
    if (!folder_name) {
        fprintf(stderr, "Error: Invalid folder path\n");
        log_operation("delete_folder", "Invalid folder path", 0);
        return;
    }

    // Check if path exists
    if (!path_exists(folder_name)) {
        fprintf(stderr, "Error: Folder '%s' does not exist\n", folder_name);
        log_operation("delete_folder", "Folder does not exist", 0);
        return;
    }

    // Check if it's actually a directory
    if (!is_directory(folder_name)) {
        fprintf(stderr, "Error: '%s' is not a directory\n", folder_name);
        log_operation("delete_folder", "Path is not a directory", 0);
        return;
    }

    DIR *dir = opendir(folder_name);
    if (!dir) {
        perror("Error opening directory");
        log_operation("delete_folder", "Failed to open directory", 0);
        return;
    }

    struct dirent *entry;
    char path[PATH_MAX];
    int has_contents = 0;

    // First, check if directory has contents
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            has_contents = 1;
            break;
        }
    }

    closedir(dir);

    // If directory has contents, ask for confirmation
    if (has_contents) {
        char response[4];
        printf("Warning: Folder '%s' contains files or directories.\n", folder_name);
        printf("This operation cannot be undone. Continue? (y/n): ");
        if (fgets(response, sizeof(response), stdin) == NULL || 
            (response[0] != 'y' && response[0] != 'Y')) {
            printf("Operation cancelled.\n");
            log_operation("delete_folder", "Operation cancelled by user", 0);
            return;
        }
    }

    // Reopen directory for deletion
    dir = opendir(folder_name);
    if (!dir) {
        perror("Error opening directory");
        log_operation("delete_folder", "Failed to open directory", 0);
        return;
    }

    // Delete contents
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (!safe_path_concat(path, sizeof(path), folder_name, entry->d_name)) {
            fprintf(stderr, "Error: Path too long\n");
            continue;
        }

        struct stat st;
        if (stat(path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                delete_folder(path);
            } else {
                if (unlink(path) != 0) {
                    perror("Error deleting file");
                    log_operation("delete_folder", path, 0);
                } else {
                    printf("Deleted file: %s\n", path);
                    log_operation("delete_folder", path, 1);
                }
            }
        }
    }

    closedir(dir);

    // Finally, delete the empty directory
    if (rmdir(folder_name) != 0) {
        perror("Error deleting directory");
        log_operation("delete_folder", folder_name, 0);
    } else {
        printf("Deleted folder: %s\n", folder_name);
        log_operation("delete_folder", folder_name, 1);
    }
}

// Function to list directory contents
void list_directory(const char *path) {
    const char *directory_path = path && strlen(path) > 0 ? path : ".";

    // Check if path exists
    if (!path_exists(directory_path)) {
        fprintf(stderr, "Error: Directory '%s' does not exist\n", directory_path);
        log_operation("list_directory", "Directory does not exist", 0);
        return;
    }

    // Check if it's actually a directory
    if (!is_directory(directory_path)) {
        fprintf(stderr, "Error: '%s' is not a directory\n", directory_path);
        log_operation("list_directory", "Path is not a directory", 0);
        return;
    }

    DIR *dir = opendir(directory_path);
    if (!dir) {
        perror("Error opening directory");
        log_operation("list_directory", "Failed to open directory", 0);
        return;
    }

    struct dirent *entry;
    struct stat file_stat;
    char full_path[PATH_MAX];
    int total_files = 0;
    off_t total_size = 0;

    printf("\nContents of directory: %s\n", directory_path);
    printf("---------------------------------------------------------------\n");
    printf("%-30s %-12s %-12s %s\n", "Name", "Type", "Size", "Permissions");
    printf("---------------------------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (!safe_path_concat(full_path, sizeof(full_path), directory_path, entry->d_name)) {
            fprintf(stderr, "Error: Path too long for '%s'\n", entry->d_name);
            continue;
        }

        if (stat(full_path, &file_stat) == 0) {
            char *file_type = "Unknown";
            if (S_ISREG(file_stat.st_mode)) file_type = "File";
            else if (S_ISDIR(file_stat.st_mode)) file_type = "Directory";
            else if (S_ISLNK(file_stat.st_mode)) file_type = "Link";

            printf("%-30s %-12s %-12ld ", 
                   entry->d_name, 
                   file_type, 
                   (long)file_stat.st_size);
            print_permissions(file_stat.st_mode);
            printf("\n");

            total_files++;
            total_size += file_stat.st_size;
        } else {
            perror("Error getting file info");
            log_operation("list_directory", full_path, 0);
        }
    }

    printf("---------------------------------------------------------------\n");
    printf("Total: %d items, %ld bytes (%.2f MB)\n", 
           total_files, 
           (long)total_size, 
           (float)total_size / (1024 * 1024));

    closedir(dir);
    log_operation("list_directory", directory_path, 1);
}

// Function to create a file
void create_file(const char *filename) {
    if (!filename) {
        fprintf(stderr, "Error: Invalid filename\n");
        log_operation("create_file", "Invalid filename", 0);
        return;
    }

    // Check if file already exists
    if (path_exists(filename)) {
        char response[4];
        printf("File '%s' already exists. Overwrite? (y/n): ", filename);
        if (fgets(response, sizeof(response), stdin) == NULL || 
            (response[0] != 'y' && response[0] != 'Y')) {
            printf("Operation cancelled.\n");
            log_operation("create_file", "Operation cancelled by user", 0);
            return;
        }
    }

    // Create file with proper permissions
    int fd = creat(filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("Error creating file");
        log_operation("create_file", filename, 0);
        return;
    }

    close(fd);
    printf("File '%s' created successfully.\n", filename);
    log_operation("create_file", filename, 1);
}

// Function to create a directory
void create_directory(const char *dirname) {
    if (!dirname) {
        fprintf(stderr, "Error: Invalid directory name\n");
        log_operation("create_directory", "Invalid directory name", 0);
        return;
    }

    // Check if directory already exists
    if (path_exists(dirname)) {
        fprintf(stderr, "Error: Directory '%s' already exists\n", dirname);
        log_operation("create_directory", "Directory already exists", 0);
        return;
    }

#ifdef _WIN32
    if (_mkdir(dirname) != 0) {
#else
    if (mkdir(dirname, 0755) == -1) {
#endif
        perror("Error creating directory");
        log_operation("create_directory", dirname, 0);
        return;
    }

    printf("Directory '%s' created successfully.\n", dirname);
    log_operation("create_directory", dirname, 1);
}

// Function to delete a file
void delete_file(const char *file_name) {
    if (!file_name) {
        fprintf(stderr, "Error: Invalid filename\n");
        log_operation("delete_file", "Invalid filename", 0);
        return;
    }

    // Check if file exists
    if (!path_exists(file_name)) {
        fprintf(stderr, "Error: File '%s' does not exist\n", file_name);
        log_operation("delete_file", "File does not exist", 0);
        return;
    }

    // Check if it's actually a file (not a directory)
    struct stat st;
    if (stat(file_name, &st) == 0 && S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Error: '%s' is a directory. Use 'sremovefolder' instead.\n", file_name);
        log_operation("delete_file", "Path is a directory", 0);
        return;
    }

    // Ask for confirmation
    char response[4];
    printf("Are you sure you want to delete '%s'? (y/n): ", file_name);
    if (fgets(response, sizeof(response), stdin) == NULL || 
        (response[0] != 'y' && response[0] != 'Y')) {
        printf("Operation cancelled.\n");
        log_operation("delete_file", "Operation cancelled by user", 0);
        return;
    }

    if (unlink(file_name) != 0) {
        perror("Error deleting file");
        log_operation("delete_file", file_name, 0);
    } else {
        printf("Deleted file: %s\n", file_name);
        log_operation("delete_file", file_name, 1);
    }
}

// Function to print working directory
void print_working_directory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current Working Directory: %s\n", cwd);
        log_operation("print_working_directory", cwd, 1);
    } else {
        perror("Error getting current working directory");
        log_operation("print_working_directory", "Failed to get cwd", 0);
    }
}

// Function to clear the console screen
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    print_file_manager_logo();
    log_operation("clear_screen", "Cleared console screen", 1);
}
