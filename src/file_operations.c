#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "file_manager.h"

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