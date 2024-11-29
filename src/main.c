#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_manager_logo.h"
#include "file_manager.h"

int main() {
    // Clear terminal screen
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    char command[256];
    char arg1[256], arg2[256];
    int args;

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

        // Parse the command and arguments
        args = sscanf(command, "%s %s %s", arg1, arg2, command);
        if (args < 1) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        // Process commands
        if (strcmp(arg1, "slist") == 0 && args == 2) {
            list_directory(arg2);
        } else if (strcmp(arg1, "scopy") == 0 && args == 3) {
            copy_file(arg2, command);
        } else if (strcmp(arg1, "sremove") == 0 && args == 2) {
            delete_folder(arg2);
        } else if (strcmp(arg1, "screatefile") == 0 && args == 2) {
            create_file(arg2); // Create file
        } else if (strcmp(arg1, "screatedir") == 0 && args == 2) {
            create_directory(arg2); // Create directory
        }else if (strcmp(arg1, "sperm") == 0 && args == 3) {
          mode_t mode = strtol(command, NULL, 8);  
          change_permissions(arg2, mode);  /
}


 else {
            printf("Invalid command or arguments. Type 'exit' to quit.\n");
        }
    }

    return 0;
}
