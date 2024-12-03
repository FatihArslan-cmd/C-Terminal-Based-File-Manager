#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "file_manager.h"

// Helper function to validate permission mode
static int validate_mode(mode_t mode) {
    // Check if mode is within valid octal range (000-777)
    return (mode >= 0 && mode <= 0777);
}

// Helper function to check if a path exists
static int path_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

// Function to change file/directory permissions
void change_permissions(const char *path, mode_t mode) {
    if (!path) {
        fprintf(stderr, "Error: Invalid path\n");
        log_operation("change_permissions", "Invalid path", 0);
        return;
    }

    // Check if path exists
    if (!path_exists(path)) {
        fprintf(stderr, "Error: Path '%s' does not exist\n", path);
        log_operation("change_permissions", "Path does not exist", 0);
        return;
    }

    // Validate permission mode
    if (!validate_mode(mode)) {
        fprintf(stderr, "Error: Invalid permission mode (must be between 000 and 777)\n");
        log_operation("change_permissions", "Invalid permission mode", 0);
        return;
    }

    // Get current permissions
    struct stat st;
    if (stat(path, &st) == 0) {
        mode_t old_mode = st.st_mode & 0777;
        
        // Change permissions
        if (chmod(path, mode) == 0) {
            printf("Permissions changed for '%s':\n", path);
            printf("  Old permissions: ");
            print_permissions(old_mode);
            printf(" (%03o)\n", old_mode);
            printf("  New permissions: ");
            print_permissions(mode);
            printf(" (%03o)\n", mode);
            
            log_operation("change_permissions", "Permissions changed successfully", 1);
        } else {
            perror("Error changing permissions");
            log_operation("change_permissions", strerror(errno), 0);
        }
    } else {
        perror("Error getting file information");
        log_operation("change_permissions", "Failed to get file information", 0);
    }
}

// Function to print permissions in human-readable format
void print_permissions(mode_t mode) {
    const char *rwx[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    char perms[10];

    // Owner permissions
    strcpy(perms, rwx[(mode >> 6) & 0x7]);
    
    // Group permissions
    strcat(perms, rwx[(mode >> 3) & 0x7]);
    
    // Others permissions
    strcat(perms, rwx[mode & 0x7]);

    printf("%s", perms);
}