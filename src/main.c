#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "file_manager_logo.h"
#include "file_manager.h"

// Function prototypes
void process_command(const char *command);
void print_invalid_command();

int main() {
    system("clear"); // Clears the screen on Unix/Linux 

    char command[256];

    print_file_manager_logo(); // Logo and description

    while (1) {
        printf("> "); // Prompt for user input
        fflush(stdout);

        // Read command and arguments from user
        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("Error reading input. Try again.\n");
            continue;
        }

        // Remove newline character from the end of command
        command[strcspn(command, "\n")] = '\0';

        // Exit condition
        if (strcmp(command, "exit") == 0) {
            printf("Exiting the File Manager. Goodbye!\n");
            break;
        }

        // Process the command
        process_command(command);
    }

    return 0;
}

void process_command(const char *command) {
    char arg1[256], arg2[256], arg3[256];
    int args = sscanf(command, "%s %s %s", arg1, arg2, arg3);

    if (args < 1) {
        print_invalid_command();
        return;
    }

    if (strcmp(arg1, "slist") == 0) {
        list_directory(args == 2 ? arg2 : ".");
    } else if (strcmp(arg1, "spwd") == 0 && args == 1) {
        print_working_directory();
    } else if (strcmp(arg1, "scopy") == 0 && args == 3) {
        copy_file(arg2, arg3);
    } else if (strcmp(arg1, "sremovefolder") == 0 && args == 2) {
        delete_folder(arg2);
    } else if (strcmp(arg1, "sremovefile") == 0 && args == 2) {
        delete_file(arg2);
    } else if (strcmp(arg1, "screatefile") == 0 && args == 2) {
        create_file(arg2);
    } else if (strcmp(arg1, "screatedir") == 0 && args == 2) {
        create_directory(arg2);
    } else if (strcmp(arg1, "spermission") == 0 && args == 3) {
        mode_t mode = strtol(arg3, NULL, 8);
        change_permissions(arg2, mode);
    } else if (strcmp(arg1, "smove") == 0 && args == 3) {
        move_file(arg2, arg3);
    } else if (strcmp(arg1, "sdisplay") == 0 && args == 2) {
        display_file_content(arg2);
    } else if (strcmp(arg1, "ssearch") == 0 && args == 3) {
        search_files_in_directory(arg2, arg3);
    } else {
        print_invalid_command();
    }
}

void print_invalid_command() {
    printf("Invalid command or arguments. Type 'exit' to quit.\n");
}
