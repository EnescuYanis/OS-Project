#include <stdio.h>
#include <string.h>
#include <sys/stat.h> // Required for mkdir

void make_directory_with_cfg(char* filepath, char* dir_name) {
    char dir_path[1024];
    char file_path[1024];
    const char* fixed_filename = "district.cfg"; // Changed from "district" to "district.cfg"

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