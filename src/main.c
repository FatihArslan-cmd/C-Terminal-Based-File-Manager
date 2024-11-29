#include <stdio.h>
#include <string.h>
#include "file_manager.h"
void print_file_manager_logo() {
    printf(
    "  ___ ___ _    ___   __  __   _   _  _   _   ___ ___ ___ \n"
    " | __|_ _| |  | __| |  \\/  | /_\\ | \\| | /_\\ / __| __| _ \\\n"
    " | _| | || |__| _|  | |\\/| |/ _ \\| .` |/ _ \\ (_ | _||   /\n"
    " |_| |___|____|___| |_|  |_/_/ \\_\\_|\\_/_/ \\_\\___|___|_|_\\\n"
    "                                                         \n"
    );

    printf("\n");

}
int main() {
    char command[256];
    char arg1[256], arg2[256];
    int args;
    print_file_manager_logo();

    printf("Welcome to Terminal-Based File Manager! Type 'exit' to quit.\n");

    while (1) {
        printf("> "); // Kullanıcıdan komut girişi
        fflush(stdout);

        // Kullanıcıdan komut ve argümanları okuma
        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("Error reading input. Try again.\n");
            continue;
        }

        // Komut satırındaki son satır sonlandırıcı karakteri (\n) kaldırılır
        command[strcspn(command, "\n")] = '\0';

        // "exit" komutu girildiğinde programdan çıkış yapılır
        if (strcmp(command, "exit") == 0) {
            printf("Exiting the File Manager. Goodbye!\n");
            break;
        }

        // Komut ve argümanları ayrıştırma
        args = sscanf(command, "%s %s %s", arg1, arg2, command);
        if (args < 1) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        // Komutların işlenmesi
        if (strcmp(arg1, "slist") == 0 && args == 2) {
            list_directory(arg2);
        } else if (strcmp(arg1, "scopy") == 0 && args == 3) {
            copy_file(arg2, command); // İkinci ve üçüncü argüman
        } else if (strcmp(arg1, "sremove") == 0 && args == 2) {
            delete_folder(arg2);
        } else {
            printf("Invalid command or arguments. Type 'exit' to quit.\n");
        }
    }

    return 0;
}
