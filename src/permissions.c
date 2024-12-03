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

void print_permissions(mode_t mode) {
    char perms[10] = "---------";
//for Owner permissions does not warn in Windows
    if (mode & S_IRUSR) perms[0] = 'r';
    if (mode & S_IWUSR) perms[1] = 'w';
    if (mode & S_IXUSR) perms[2] = 'x';

//for group permissions warns in Windows works perfectly in Linux

    if (mode & S_IRGRP) perms[3] = 'r';
    if (mode & S_IWGRP) perms[4] = 'w';
    if (mode & S_IXGRP) perms[5] = 'x';

//for other permission warns in Windows works perfectly in Linux
 
    if (mode & S_IROTH) perms[6] = 'r';
    if (mode & S_IWOTH) perms[7] = 'w';
    if (mode & S_IXOTH) perms[8] = 'x';

    printf("%s", perms);
}