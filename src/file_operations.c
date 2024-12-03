#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "file_manager.h"

// Function to copy a file from source to destination
void copy_file(const char *source, const char *destination) {
    int src_fd = open(source, O_RDONLY);
    if (src_fd < 0) {
        perror("Source file error");
        log_operation("copy_file", "Failed to open source file", 0);  // 0 for failure
        return;
    }

    int dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Destination file error");
        log_operation("copy_file", "Failed to open destination file", 0);  // 0 for failure
        close(src_fd);
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, bytes_read) == -1) {
            perror("Error writing to destination file");
            log_operation("copy_file", "Error writing to destination file", 0);  // 0 for failure
            close(src_fd);
            close(dest_fd);
            return;
        }
    }

    close(src_fd);
    close(dest_fd);

    // Log the success with detailed information about the source and destination
    log_operation("copy_file", "File copied from source to destination", 1);  // 1 for success
    printf("File copied successfully from %s to %s.\n", source, destination);
}



// Function to move a file from source to destination
void move_file(const char *source, const char *destination) {
    // First, try to rename the file (works within the same filesystem)
    if (rename(source, destination) == 0) {
        // Log the success if rename is successful
        log_operation("move_file", "File moved using rename", 1);  // 1 for success
        printf("File successfully moved from %s to %s.\n", source, destination);
        return;
    }

    // If rename fails, try copying the file and deleting the source file
    copy_file(source, destination);  // Reuse the copy_file function
    
    // Now that the file is copied, remove the original source file
    if (unlink(source) == -1) {
        perror("Error deleting the source file");
        log_operation("move_file", "Failed to delete the source file", 0);  // 0 for failure
        return;
    }

    // Log the success with detailed information about the source and destination
    log_operation("move_file", "File moved from source to destination", 1);  // 1 for success
    printf("File successfully moved from %s to %s.\n", source, destination);
}



// Function to display the content of a file
void display_file_content(const char *file_path) {
    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open the file for reading");
        log_operation("display_file_content", "Failed to open file", 0);  // 0 for failure
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;
    printf("Displaying contents of %s:\n", file_path);
    
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        if (write(STDOUT_FILENO, buffer, bytes_read) == -1) {
            perror("Error writing to stdout");
            log_operation("display_file_content", "Error writing to stdout", 0);  // 0 for failure
            close(fd);
            return;
        }
    }

    if (bytes_read < 0) {
        perror("Error reading the file");
        log_operation("display_file_content", "Error reading the file", 0);  // 0 for failure
    }

    close(fd);

    // Log the success with detailed information about the file
    log_operation("display_file_content", "Displayed file content", 1);  // 1 for success
}



// Function to search for a string within a directory
void search_files_in_directory(const char *directory_path, const char *search_string) {
    DIR *dir = opendir(directory_path);
    if (!dir) {
        perror("Unable to open directory");
        log_operation("search_files_in_directory", "Failed to open directory", 0);  // 0 for failure
        return;
    }

    struct dirent *entry;
    int found = 0; //check if there is matched files

    printf("Searching for '%s' in directory: %s\n**\n", search_string, directory_path); 

    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a file (not a directory)
        if (entry->d_type == DT_REG) {
            if (strstr(entry->d_name, search_string) != NULL) {
                printf("Found file: %s\n", entry->d_name);
                log_operation("search_files_in_directory", "Found file matching search string", 1);  // 1 for success
                found = 1; // Dosya bulundu
            }
        }
    }

    if (!found) {
        printf("No files matching '%s' were found in the directory.\n", search_string);
        log_operation("search_files_in_directory", "No files matched search string", 0);  // 0 for failure
    }

    closedir(dir);
}