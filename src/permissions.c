#include <stdio.h>
#include <sys/stat.h>
#include "file_manager.h"

void change_permissions(const char *path, mode_t mode) {
    if (chmod(path, mode) == 0) {
        printf("Permissions changed successfully for '%s'.\n", path);
        log_operation("change_permissions", path, 1);  // 1 for success
    } else {
        perror("Failed to change permissions");
        log_operation("change_permissions", path, 0);  // 0 for failure
    }
}
