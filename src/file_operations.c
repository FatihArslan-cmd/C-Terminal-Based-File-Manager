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
        return;
    }

    int dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Destination file error");
        close(src_fd);
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        write(dest_fd, buffer, bytes_read);
    }

    close(src_fd);
    close(dest_fd);
    printf("File copied successfully.\n");
}
