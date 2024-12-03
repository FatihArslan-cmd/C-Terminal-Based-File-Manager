#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include "file_manager_logo.h"
#include "file_manager.h"

#define MAX_COMMAND_LENGTH 4096
#define MAX_ARGS 3
#define MAX_ARG_LENGTH 1024

// Function prototypes
void process_command(const char *command);
void print_invalid_command(const char *reason);
int validate_path(const char *path);
void trim_whitespace(char *str);

int main() {
    // Clear screen and show logo
    clear_screen();

    char *command = malloc(MAX_COMMAND_LENGTH);
    if (command == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }

    while (1) {
        printf("> ");
        fflush(stdout);
        memset(command, 0, MAX_COMMAND_LENGTH);

        // Read command safely
        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            if (feof(stdin)) {
                printf("\nEnd of input. Exiting...\n");
                free(command);
                return 0;
            }
            printf("Error reading input. Try again.\n");
            continue;
        }

        // Remove trailing newline
        size_t len = strlen(command);
        if (len > 0 && command[len-1] == '\n') {
            command[len-1] = '\0';
        }

        // Check for buffer overflow
        if (len == MAX_COMMAND_LENGTH - 1 && command[len-1] != '\n') {
            fprintf(stderr, "Error: Command too long\n");
            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // Trim whitespace
        trim_whitespace(command);

        // Exit condition
        if (strcmp(command, "exit") == 0) {
            printf("Exiting the File Manager. Goodbye!\n");
            break;
        }

        // Process the command
        process_command(command);
    }

    free(command);
    return 0;
}

void process_command(const char *command) {
    if (command == NULL || strlen(command) == 0) {
        print_invalid_command("Empty command");
        return;
    }

    char *args[MAX_ARGS];
    for (int i = 0; i < MAX_ARGS; i++) {
        args[i] = malloc(MAX_ARG_LENGTH);
        if (args[i] == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(args[j]);
            }
            return;
        }
        memset(args[i], 0, MAX_ARG_LENGTH);
    }

    // Parse command safely
    char *cmd_copy = strdup(command);
    if (cmd_copy == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        for (int i = 0; i < MAX_ARGS; i++) {
            free(args[i]);
        }
        return;
    }

    char *token = strtok(cmd_copy, " ");
    int arg_count = 0;

    while (token != NULL && arg_count < MAX_ARGS) {
        if (strlen(token) < MAX_ARG_LENGTH) {
            strncpy(args[arg_count], token, MAX_ARG_LENGTH - 1);
            args[arg_count][MAX_ARG_LENGTH - 1] = '\0';
            arg_count++;
        }
        token = strtok(NULL, " ");
    }

    free(cmd_copy);

    // Command processing with validation
    if (strcmp(args[0], "slist") == 0) {
        list_directory(arg_count == 2 && validate_path(args[1]) ? args[1] : ".");
    } else if (strcmp(args[0], "spwd") == 0 && arg_count == 1) {
        print_working_directory();
    } else if (strcmp(args[0], "scopy") == 0 && arg_count == 3) {
        if (validate_path(args[1]) && validate_path(args[2])) {
            copy_file(args[1], args[2]);
        } else {
            print_invalid_command("Invalid path specified");
        }
    } else if (strcmp(args[0], "sremovefolder") == 0 && arg_count == 2) {
        if (validate_path(args[1])) {
            delete_folder(args[1]);
        } else {
            print_invalid_command("Invalid path specified");
        }
    } else if (strcmp(args[0], "sremovefile") == 0 && arg_count == 2) {
        if (validate_path(args[1])) {
            delete_file(args[1]);
        } else {
            print_invalid_command("Invalid path specified");
        }
    } else if (strcmp(args[0], "screatefile") == 0 && arg_count == 2) {
        if (validate_path(args[1])) {
            create_file(args[1]);
        } else {
            print_invalid_command("Invalid path specified");
        }
    } else if (strcmp(args[0], "screatedir") == 0 && arg_count == 2) {
        if (validate_path(args[1])) {
            create_directory(args[1]);
        } else {
            print_invalid_command("Invalid path specified");
        }
    } else if (strcmp(args[0], "spermission") == 0 && arg_count == 3) {
        if (validate_path(args[1])) {
            // Validate permission format (octal)
            char *endptr;
            long mode = strtol(args[2], &endptr, 8);
            if (*endptr != '\0' || mode < 0 || mode > 0777) {
                print_invalid_command("Invalid permission format (use octal: 644, 755, etc.)");
            } else {
                change_permissions(args[1], (mode_t)mode);
            }
        } else {
            print_invalid_command("Invalid path specified");
        }
    } else if (strcmp(args[0], "smove") == 0 && arg_count == 3) {
        if (validate_path(args[1]) && validate_path(args[2])) {
            move_file(args[1], args[2]);
        } else {
            print_invalid_command("Invalid path specified");
        }
    } else if (strcmp(args[0], "sdisplay") == 0 && arg_count == 2) {
        if (validate_path(args[1])) {
            display_file_content(args[1]);
        } else {
            print_invalid_command("Invalid path specified");
        }
    } else if (strcmp(args[0], "ssearch") == 0 && arg_count == 3) {
        if (validate_path(args[1])) {
            search_files_in_directory(args[1], args[2]);
        } else {
            print_invalid_command("Invalid path specified");
        }
    } else if (strcmp(args[0], "sclear") == 0 && arg_count == 1) {
        clear_screen();
    } else {
        print_invalid_command("Unknown command or invalid number of arguments");
    }

    // Free allocated memory
    for (int i = 0; i < MAX_ARGS; i++) {
        free(args[i]);
    }
}

void print_invalid_command(const char *reason) {
    fprintf(stderr, "Error: %s\n", reason);
    printf("Type 'exit' to quit or use one of the following commands:\n");
    print_file_manager_logo();
}

int validate_path(const char *path) {
    if (path == NULL || strlen(path) == 0) {
        return 0;
    }

    // Check for path length
    if (strlen(path) > PATH_MAX) {
        return 0;
    }

    // Check for basic path traversal attempts
    if (strstr(path, "..") != NULL) {
        return 0;
    }

    // Check for invalid characters
    const char *invalid_chars = "<>:\"|?*";
    if (strpbrk(path, invalid_chars) != NULL) {
        return 0;
    }

    return 1;
}

void trim_whitespace(char *str) {
    if (str == NULL) {
        return;
    }

    // Trim leading space
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // All spaces?
    if (*str == 0) {
        return;
    }

    // Trim trailing space
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    end[1] = '\0';
}
