#include <stdio.h>
#include "file_manager_logo.h"

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"

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
    printf(GREEN "- slist " YELLOW "<directory>" RESET ": List contents of a directory.\n");
    printf(GREEN "- scopy " YELLOW "<source> <destination>" RESET ": Copy a file.\n");
    printf(GREEN "- sremovefolder " YELLOW "<directory/file>" RESET ": Remove a directory.\n");
    printf(GREEN "- sremovefile " YELLOW "<file>" RESET ": Remove a file.\n");
    printf(GREEN "- screatefile " YELLOW "<file>" RESET ": Create a new file.\n");
    printf(GREEN "- screatedir " YELLOW "<directory>" RESET ": Create a new directory.\n");
    printf(GREEN "- sperm " YELLOW "<file/directory> <permissions>" RESET ": Change permissions of a file or directory (755 777 644 600).\n");
    printf(GREEN "- smove " YELLOW "<source> <destination>" RESET ": Move a file (rename supported).\n");
    printf(GREEN "- sdisplay " YELLOW "<file>" RESET ": Display the contents of a file.\n");
    printf(GREEN "- ssearch " YELLOW "<directory> <file>" RESET ": Search for a specific file in a directory.\n");
    printf(GREEN "- exit: " RESET "Quit the program.\n\n");
}
