#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <sys/types.h> // mode_t için gerekli

void list_directory(const char *path);
void copy_file(const char *source, const char *destination);
void delete_folder(const char *folder_name);
void change_permissions(const char *path, mode_t mode);
void log_operation(const char *operation, const char *details, int success);  // Keep this declaration only once
void create_file(const char *filename);
void create_directory(const char *dirname);
void move_file(const char *source, const char *destination);
void display_file_content(const char *file_path);
#endif // FILE_MANAGER_H
