#include <stdio.h>
#include <string.h>
#include "file_manager.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: [command] [arguments]\n");
        return 1;
    }

    if (strcmp(argv[1], "slist") == 0 && argc == 3) {
        list_directory(argv[2]);
    } else if (strcmp(argv[1], "scopy") == 0 && argc == 4) {
        copy_file(argv[2], argv[3]);
    } else if (strcmp(argv[1], "sremove") == 0 && argc == 3) {
        delete_folder(argv[2]); 
    } else {
        printf("Invalid command or arguments.\n");
    }

    return 0;
}
