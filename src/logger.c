#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "file_manager.h"

void log_operation(const char *operation, const char *details, int success) {
    int log_fd = open("file_manager.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        perror("Log file error");
        return;
    }

    char buffer[256];
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    snprintf(buffer, sizeof(buffer), "[%02d-%02d-%04d %02d:%02d:%02d] %s: %s - %s\n",
             local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900,
             local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
             operation, details, success ? "SUCCESS" : "FAILURE");

    write(log_fd, buffer, strlen(buffer));
    close(log_fd);
}
