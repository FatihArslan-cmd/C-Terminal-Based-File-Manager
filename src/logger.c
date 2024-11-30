#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "file_manager.h"

// Function to write logs to a file (updated to match header)
void log_operation(const char *operation, const char *details, int success) {
    // Open log file for appending (create if not exist)
    int log_fd = open("app_log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        perror("Error opening log file");
        return;
    }

    // Get current date and time
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[100];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);

    // Prepare log entry
    char log_entry[512];
    const char *result = success ? "Success" : "Failure"; // Use success flag to determine result
    snprintf(log_entry, sizeof(log_entry), "[%s] Operation: %s, Details: %s, Result: %s\n", time_str, operation, details, result);

    // Write the log entry to the log file
    if (write(log_fd, log_entry, strlen(log_entry)) == -1) {
        perror("Error writing to log file");
    }

    // Close the log file
    close(log_fd);
}
