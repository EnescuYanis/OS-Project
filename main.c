#include "commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Required for srand()
#include <time.h>   // Required for time()

int main(int argc, char *argv[]) {
    // 1. Seed the random number generator ONCE at the start of the program
    srand(time(NULL));

    if (argc < 2) {
        printf("Error: No command provided.\n Might be useful to run 'help' for more information.\n");
        handle_help();
        return 1;
    }

    const char *command = argv[1];

    if (strcmp(command, "add") == 0) {
        // Now requires 4 arguments: ./script add <folder> <filename>
        if (argc < 4) {
            printf("Error: The 'add' command requires a directory and a filename.\n");
            printf("Usage: ./my_tool add <folder_name> <file_name.dat>\n");
            return 1;
        }
        
        // Pass argv[2] (folder) and argv[3] (file name)
        handle_add(argv[2], argv[3]);
        
    } else if (strcmp(command, "remove") == 0) {
        if (argc < 3) {
            printf("Error: The 'remove' command requires a target name.\n");
            return 1;
        }
        handle_remove(argv[2]);
        
    } else if (strcmp(command, "help") == 0) {
        handle_help();
    } else if (strcmp(command, "list") == 0) {
            if (argc < 3) {
                printf("Error: The 'list' command requires a district ID.\n");
                printf("Usage: ./my_tool list <district_id>\n");
                return 1;
            }
            handle_list(argv[2]);
        // -----------------------------

        
    } 
    else if (strcmp(command, "view") == 0) {
        // Needs 4 arguments: ./my_tool view sector_7 12345
        if (argc < 4) {
            printf("Error: The 'view' command requires a district ID and a report ID.\n");
            printf("Usage: ./my_tool view <district_id> <report_id>\n");
            return 1;
        }
        handle_view(argv[2], argv[3]);}    
    else {
        printf("Error: Unknown command '%s'.\n", command);
        handle_help();
        return 1;
    }

    return 0;
}