#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "file_manager.h"

#define BUFFER_SIZE 8192  // Optimal buffer size for file operations

// Helper function to check if a file exists
static int file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

// Helper function to get file size
static off_t get_file_size(int fd) {
    struct stat st;
    if (fstat(fd, &st) == 0) {
        return st.st_size;
    }
    return -1;
}

// Function to copy a file from source to destination with progress
void copy_file(const char *source, const char *destination) {
    // Validate inputs
    if (!source || !destination) {
        fprintf(stderr, "Error: Invalid source or destination path\n");
        log_operation("copy_file", "Invalid parameters", 0);
        return;
    }

    // Check if source exists
    if (!file_exists(source)) {
        fprintf(stderr, "Error: Source file '%s' does not exist\n", source);
        log_operation("copy_file", "Source file does not exist", 0);
        return;
    }

    // Check if destination already exists
    if (file_exists(destination)) {
        char response[4];
        printf("File '%s' already exists. Overwrite? (y/n): ", destination);
        if (fgets(response, sizeof(response), stdin) == NULL || 
            (response[0] != 'y' && response[0] != 'Y')) {
            printf("Operation cancelled.\n");
            log_operation("copy_file", "Operation cancelled by user", 0);
            return;
        }
    }

    // Open source file
    int src_fd = open(source, O_RDONLY);
    if (src_fd < 0) {
        perror("Error opening source file");
        log_operation("copy_file", "Failed to open source file", 0);
        return;
    }

    // Get source file size for progress reporting
    off_t total_size = get_file_size(src_fd);
    off_t bytes_copied = 0;

    // Open destination file with proper permissions
    int dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Error opening destination file");
        log_operation("copy_file", "Failed to open destination file", 0);
        close(src_fd);
        return;
    }

    // Allocate buffer
    char *buffer = malloc(BUFFER_SIZE);
    if (!buffer) {
        perror("Error allocating memory");
        log_operation("copy_file", "Memory allocation failed", 0);
        close(src_fd);
        close(dest_fd);
        return;
    }

    // Copy file contents with progress indication
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        ssize_t bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to destination file");
            log_operation("copy_file", "Write error", 0);
            free(buffer);
            close(src_fd);
            close(dest_fd);
            return;
        }
        bytes_copied += bytes_written;
        
        // Show progress
        if (total_size > 0) {
            float progress = ((float)bytes_copied / total_size) * 100;
            printf("\rCopying: %.1f%% complete", progress);
            fflush(stdout);
        }
    }

    if (bytes_read < 0) {
        perror("Error reading from source file");
        log_operation("copy_file", "Read error", 0);
    } else {
        printf("\nFile copied successfully.\n");
        log_operation("copy_file", "File copied successfully", 1);
    }

    // Cleanup
    free(buffer);
    close(src_fd);
    close(dest_fd);
}

// Function to move a file from source to destination
void move_file(const char *source, const char *destination) {
    if (!source || !destination) {
        fprintf(stderr, "Error: Invalid source or destination path\n");
        log_operation("move_file", "Invalid parameters", 0);
        return;
    }

    // Check if source exists
    if (!file_exists(source)) {
        fprintf(stderr, "Error: Source file '%s' does not exist\n", source);
        log_operation("move_file", "Source file does not exist", 0);
        return;
    }

    // Check if destination already exists
    if (file_exists(destination)) {
        char response[4];
        printf("File '%s' already exists. Overwrite? (y/n): ", destination);
        if (fgets(response, sizeof(response), stdin) == NULL || 
            (response[0] != 'y' && response[0] != 'Y')) {
            printf("Operation cancelled.\n");
            log_operation("move_file", "Operation cancelled by user", 0);
            return;
        }
    }

    // First try to rename (move) the file
    if (rename(source, destination) == 0) {
        printf("File moved successfully.\n");
        log_operation("move_file", "File moved using rename", 1);
        return;
    }

    // If rename fails (e.g., across different filesystems), try copy and delete
    if (errno == EXDEV) {
        printf("Moving file across filesystems...\n");
        copy_file(source, destination);
        
        // Only delete source if copy was successful
        if (file_exists(destination)) {
            if (unlink(source) == 0) {
                printf("File moved successfully.\n");
                log_operation("move_file", "File moved using copy and delete", 1);
            } else {
                perror("Warning: Could not delete source file");
                log_operation("move_file", "Source file not deleted after copy", 0);
            }
        }
    } else {
        perror("Error moving file");
        log_operation("move_file", "Move operation failed", 0);
    }
}

// Function to display the content of a file
void display_file_content(const char *file_path) {
    if (!file_path) {
        fprintf(stderr, "Error: Invalid file path\n");
        log_operation("display_file_content", "Invalid file path", 0);
        return;
    }

    // Check if file exists
    if (!file_exists(file_path)) {
        fprintf(stderr, "Error: File '%s' does not exist\n", file_path);
        log_operation("display_file_content", "File does not exist", 0);
        return;
    }

    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        log_operation("display_file_content", "Failed to open file", 0);
        return;
    }

    // Get file size
    off_t file_size = get_file_size(fd);
    if (file_size > 1024 * 1024) { // 1MB
        char response[4];
        printf("File is large (%.2f MB). Display anyway? (y/n): ", 
               (float)file_size / (1024 * 1024));
        if (fgets(response, sizeof(response), stdin) == NULL || 
            (response[0] != 'y' && response[0] != 'Y')) {
            printf("Operation cancelled.\n");
            close(fd);
            log_operation("display_file_content", "Operation cancelled by user", 0);
            return;
        }
    }

    char *buffer = malloc(BUFFER_SIZE);
    if (!buffer) {
        perror("Error allocating memory");
        close(fd);
        log_operation("display_file_content", "Memory allocation failed", 0);
        return;
    }

    printf("\nContents of %s:\n", file_path);
    printf("---------------------------------------------------------------\n");

    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
            perror("Error writing to stdout");
            free(buffer);
            close(fd);
            log_operation("display_file_content", "Error writing to stdout", 0);
            return;
        }
    }

    if (bytes_read < 0) {
        perror("Error reading file");
        log_operation("display_file_content", "Error reading file", 0);
    } else {
        printf("\n---------------------------------------------------------------\n");
        log_operation("display_file_content", "File contents displayed", 1);
    }

    free(buffer);
    close(fd);
}

// Function to search for files in a directory
void search_files_in_directory(const char *directory_path, const char *search_string) {
    if (!directory_path || !search_string) {
        fprintf(stderr, "Error: Invalid parameters\n");
        log_operation("search_files_in_directory", "Invalid parameters", 0);
        return;
    }

    DIR *dir = opendir(directory_path);
    if (!dir) {
        perror("Error opening directory");
        log_operation("search_files_in_directory", "Failed to open directory", 0);
        return;
    }

    struct dirent *entry;
    int found = 0;
    char full_path[PATH_MAX];

    printf("Searching for '%s' in directory: %s\n", search_string, directory_path);
    printf("---------------------------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct full path
        snprintf(full_path, sizeof(full_path), "%s/%s", directory_path, entry->d_name);

        // Get file information
        struct stat st;
        if (stat(full_path, &st) == 0) {
            if (strstr(entry->d_name, search_string) != NULL) {
                printf("Found: %s\n", entry->d_name);
                printf("  Type: %s\n", S_ISDIR(st.st_mode) ? "Directory" : "File");
                printf("  Size: %ld bytes\n", (long)st.st_size);
                printf("  Permissions: ");
                print_permissions(st.st_mode);
                printf("\n---------------------------------------------------------------\n");
                found = 1;
            }
        }
    }

    if (!found) {
        printf("No files matching '%s' were found.\n", search_string);
        log_operation("search_files_in_directory", "No matching files found", 0);
    } else {
        log_operation("search_files_in_directory", "Search completed successfully", 1);
    }

    closedir(dir);
}

