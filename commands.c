#include "commands.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h> 
#include <unistd.h> 
// Required for mkdir

// --- Function Implementations ---

void make_directory_with_cfg(const char* filepath, const char* dir_name) {
    char dir_path[1024];
    char file_path[2048];
    const char* fixed_filename = "district.cfg";

    // 1. Construct the directory path (e.g., "./new_folder")
    snprintf(dir_path, sizeof(dir_path), "%s/%s", filepath, dir_name);

    // 2. Attempt to create the directory
    if (mkdir(dir_path, 0755) == 0) {
        printf("Directory created: %s\n", dir_path);
        
        // 3. Construct the file path (e.g., "./new_folder/district.cfg")
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, fixed_filename);

        // 4. Create the .cfg file inside the new directory
        FILE *file = fopen(file_path, "w");
        if (file != NULL) {
            fprintf(file, "# Configuration file: %s\n", fixed_filename);
            fclose(file);
            printf("File created: %s\n", file_path);
        } else {
            perror("Error creating .cfg file");
        }
    } else {
        perror("Error creating directory");
    }
}

void handle_add(const char *dir_name, const char *file_name) {
    // Bold Cyan for the main execution header
    printf(STYLE_BOLD COLOR_CYAN "Action: Executing ADD logic...\n" COLOR_RESET);
    
    spinner_loading(); 

    struct stat st = {0};
    char file_path[2048]; 

    // 1. Check if the target directory exists
    if (stat(dir_name, &st) == -1) {
        // Yellow for a notice/warning
        printf(COLOR_YELLOW "\nDirectory '%s' does not exist. Creating it now...\n" COLOR_RESET, dir_name);
        make_directory_with_cfg(".", dir_name); 
    } else {
        // Cyan for a standard informational message
        printf(COLOR_CYAN "\nDirectory '%s' found.\n" COLOR_RESET, dir_name);
    }

    // 2. Construct the path
    snprintf(file_path, sizeof(file_path), "%s/%s", dir_name, file_name);

    // 3. Open the file in append mode ("a") and close it immediately
    FILE *file = fopen(file_path, "a");
    if (file != NULL) {
        fclose(file); 
        // Green [SUCCESS] tag
        printf(COLOR_GREEN "  [SUCCESS] " COLOR_RESET "Empty file created: %s\n", file_path);
    } else {
        // Red [ERROR] tag
        printf(COLOR_RED "  [ERROR] " COLOR_RESET "Failed to create file: %s\n", file_path);
        return; 
    }

    // 4. Set permissions to 664
    if (chmod(file_path, 0664) == 0) {
        printf(COLOR_GREEN "  [SUCCESS] " COLOR_RESET "Permissions for %s set to 664.\n", file_path);
    } else {
        printf(COLOR_RED "  [ERROR] " COLOR_RESET "Failed to set file permissions for %s\n", file_path);
    }

    printf("\n");
}

void handle_remove(const char *filename) {
    printf("Action: Executing REMOVE logic...\n");
    printf("Target: %s\n", filename);
    // TODO: Put your actual logic for removing the directory/file here
}

void handle_help(void) {
    printf("\nAvailable Commands:\n");
    printf("  add <dirname> <filename.dat>   : Adds a report.dat file to a directory and a district.cfg by default.\n");
    printf("  remove <target>                : Removes the specified target.\n");
    printf("  help                           : Displays this menu.\n\n");
}


void spinner_loading() {
    const char spinner_chars[] = "|/-\\";
    int num_chars = 4;
    
    // We want to load for 2 seconds total.
    // Let's update the spinner every 0.1 seconds (100,000 microseconds).
    // 2.0s / 0.1s = 20 loops.
    
    for (int i = 0; i < 10; i++) {
        // \r moves the cursor back to the start of the line
        printf("\rExecuting command... %c", spinner_chars[i % num_chars]);
        fflush(stdout);
        
        usleep(100000); // Wait for 100,000 microseconds (0.1 seconds)
    }
    
    // Clear the line and print the final message
    printf("\rExecution Complete!        \n"); 
}