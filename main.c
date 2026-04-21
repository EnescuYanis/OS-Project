#include "commands.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
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
        
    } else {
        printf("Error: Unknown command '%s'.\n", command);
        handle_help();
        return 1;
    }

    return 0;
}