#include <stdio.h>
#include "file_manager_logo.h"

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"

void print_file_manager_logo() {
    printf(GREEN
    "  ___ ___ _    ___   __  __   _   _  _   _   ___ ___ ___ \n"
    " | __|_ _| |  | __| |  \\/  | /_\\ | \\| | /_\\ / __| __| _ \\\n"
    " | _| | || |__| _|  | |\\/| |/ _ \\| .` |/ _ \\ (_ | _||   /\n"
    " |_| |___|____|___| |_|  |_/_/ \\_\\_|\\_/_/ \\_\\___|___|_|_\\\n"
    "                                                         \n"
    RESET);

    printf(BLUE "Welcome to Terminal-Based File Manager!\n" RESET);
    printf("You can use the following commands:\n");
    printf(GREEN "- spwd : " RESET "Path of current working directory.\n");
    printf(GREEN "- slist <directory>: " RESET "List contents of a directory.\n");
    printf(GREEN "- scopy <source> <destination>: " RESET "Copy a file.\n");
    printf(GREEN "- sremove <directory/file>: " RESET "Remove a directory or file.\n");
    printf(GREEN "- screatefile <file>: " RESET "Create a new file.\n");
    printf(GREEN "- screatedir <directory>: " RESET "Create a new directory.\n");
    printf(GREEN "- sperm <file/directory> <permissions>: " RESET "Change permissions of a file or directory (755 777 644 600).\n");
    printf(GREEN "- smove <source> <destination>: " RESET "Move a file (rename or copy and delete).\n"); 
    printf(GREEN "- sdisplay <file>: " RESET "Display the contents of a file.\n");
    printf(GREEN "- ssearch <directory> <file>: " RESET "Search for a specific file in a directory.\n");
    printf(GREEN "- exit: " RESET "Quit the program.\n\n");
}
