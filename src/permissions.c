#include <stdio.h>
#include <sys/stat.h>
#include "file_manager.h"

void change_permissions(const char *path, mode_t mode) {
    if (chmod(path, mode) == 0) {
        printf("Permissions changed successfully.\n");
    } else {
        perror("Failed to change permissions");
    }
}
