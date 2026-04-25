#include "commands.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <stdlib.h>
#include <time.h>
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
        printf(COLOR_YELLOW "\nDirectory '%s' does not exist. Creating it now...\n" COLOR_RESET, dir_name);
        make_directory_with_cfg(".", dir_name); 
    } else {
        printf(COLOR_CYAN "\nDirectory '%s' found.\n" COLOR_RESET, dir_name);
    }

    // 2. Construct the path
    snprintf(file_path, sizeof(file_path), "%s/%s", dir_name, file_name);

    // 3. Touch the file to ensure it exists
    FILE *file = fopen(file_path, "a");
    if (file != NULL) {
        fclose(file); 
        printf(COLOR_GREEN "  [SUCCESS] " COLOR_RESET "File ready: %s\n", file_path);
    } else {
        printf(COLOR_RED "  [ERROR] " COLOR_RESET "Failed to create file: %s\n", file_path);
        return; 
    }

    // 4. Set permissions to 664
    if (chmod(file_path, 0664) == 0) {
        printf(COLOR_GREEN "  [SUCCESS] " COLOR_RESET "Permissions set to 664.\n", file_path);
    } else {
        printf(COLOR_RED "  [ERROR] " COLOR_RESET "Failed to set file permissions.\n");
    }

    // 5. Trigger the interactive prompt for the user
    printf(STYLE_BOLD COLOR_CYAN "\n--- Entering New Report Data ---\n" COLOR_RESET);
    Record new_record = add_record_file();

    // 6. Append the binary struct to the file
    add_record(file_path, &new_record);

    // Final Success Message
    printf(COLOR_GREEN "\n  [SUCCESS] " COLOR_RESET "Report ID %d saved to %s!\n\n", new_record.id, file_path);
}


void handle_remove(const char *filename) {
    printf("Action: Executing REMOVE logic...\n");
    printf("Target: %s\n", filename);
    // TODO: Put your actual logic for removing the directory/file here
}

void handle_help(void) {
    printf("\nAvailable Commands:\n");
    printf("  add <dirname> <filename.dat>   : Adds a report.dat file to a directory.\n");
    printf("  remove <target>                : Removes the specified target.\n");
    printf("  list <district_id>             : Lists all reports in the specified district.\n");
    printf("  view <district_id> <report_id> : Print the full details of a specific report.\n");
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

void add_record(const char* filename, Record* record) {
    FILE *file = fopen(filename, "ab"); // Changed to fopen
    if (file == NULL) {
        printf(COLOR_RED "  [ERROR] " COLOR_RESET "Error opening file %s for writing\n", filename);
        return;
    }

    // Write the record to the file safely
    fwrite(record, sizeof(Record), 1, file);
    fclose(file);
}

// 3. Fixed input loop, memory, and return value
Record add_record_file() {
    Record new_record;
    new_record.id = rand(); // Assumes srand() was called in main()
    
    printf("\nEnter longitude: ");
    scanf("%f", &new_record.longitude);
    
    printf("Enter latitude: ");
    scanf("%f", &new_record.latitude);  
    
    printf("Category (road/lighting/flooding/other): ");
    // %49s prevents buffer overflows by limiting input to 49 chars (+ null terminator)
    scanf("%49s", new_record.issue_category); 
    
    int security_level;
    while (1) {
        printf("Security level (1-3): ");
        scanf("%d", &security_level);
        
        if (security_level >= 1 && security_level <= 3) {
            new_record.security_level = security_level;
            break; // Exit the loop because the input is good
        }
        
        // Print a colorful warning if they messed up
        printf(COLOR_YELLOW "  Invalid level. Must be between 1 and 3.\n" COLOR_RESET);
    }
    
    new_record.timestamp = time(NULL); 
    
    // Fill out empty strings for the ones we didn't prompt for yet to avoid garbage data
    new_record.inspector_name[0] = '\0';
    new_record.description[0] = '\0';

    return new_record;
}

void get_permission_string(mode_t mode, char *str) {
    // Start with empty permissions
    strcpy(str, "---------");

    // User permissions
    if (mode & S_IRUSR) str[0] = 'r';
    if (mode & S_IWUSR) str[1] = 'w';
    if (mode & S_IXUSR) str[2] = 'x';

    // Group permissions
    if (mode & S_IRGRP) str[3] = 'r';
    if (mode & S_IWGRP) str[4] = 'w';
    if (mode & S_IXGRP) str[5] = 'x';

    // Other permissions
    if (mode & S_IROTH) str[6] = 'r';
    if (mode & S_IWOTH) str[7] = 'w';
    if (mode & S_IXOTH) str[8] = 'x';
}

// --- The List Command ---
void handle_list(const char *district_id) {
    printf(STYLE_BOLD COLOR_CYAN "Action: Executing LIST logic for %s...\n\n" COLOR_RESET, district_id);

    char file_path[2048];
    snprintf(file_path, sizeof(file_path), "%s/reports.dat", district_id);

    struct stat file_stat;

    // 1. Check if the file exists and get its metadata
    if (stat(file_path, &file_stat) == -1) {
        printf(COLOR_RED "  [ERROR] " COLOR_RESET "Cannot access reports for district '%s'.\n", district_id);
        return;
    }

    // 2. Format the Permissions
    char perms[10]; // 9 characters for rw-rw-r-- plus the null terminator
    get_permission_string(file_stat.st_mode, perms);

    // 3. Format the Modification Time
    char time_buf[64];
    struct tm *tm_info = localtime(&file_stat.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    // 4. Print the File Information Header
    printf(STYLE_BOLD "--- File Information ---\n" COLOR_RESET);
    printf("Path        : %s\n", file_path);
    printf("Permissions : %s\n", perms);
    printf("Size        : %lld bytes\n", (long long)file_stat.st_size);
    printf("Last Mod    : %s\n", time_buf);
    printf("------------------------\n\n");

    // 5. Open the file to read the binary records
    FILE *file = fopen(file_path, "rb"); // "rb" is Read Binary
    if (file == NULL) {
        printf(COLOR_RED "  [ERROR] " COLOR_RESET "Failed to open %s for reading.\n", file_path);
        return;
    }

    printf(STYLE_BOLD "--- District Reports ---\n" COLOR_RESET);
    
    Record current_record;
    int record_count = 0;

    // 6. Loop through the file, printing ONLY the ID
    while (fread(&current_record, sizeof(Record), 1, file) == 1) {
        record_count++;
        
        // Print just the ID as a bullet point
        printf(COLOR_YELLOW "  - Report ID: %d\n" COLOR_RESET, current_record.id);
    }

    if (record_count == 0) {
        printf("  No reports found in this district.\n");
    } else {
        printf(COLOR_GREEN "\n  [SUCCESS] " COLOR_RESET "Found %d report(s).\n", record_count);
    }

    fclose(file);
}

void handle_view(const char *district_id, const char *report_id) {
    char file_path[2048];
    snprintf(file_path, sizeof(file_path), "%s/reports.dat", district_id);

    // Convert the target ID from text to an integer
    int target_id = atoi(report_id);

    FILE *file = fopen(file_path, "rb"); 
    if (file == NULL) {
        printf(COLOR_RED "  [ERROR] " COLOR_RESET "Failed to open %s for reading.\n", file_path);
        return;
    }

    Record current_record;
    int found = 0;

    // Loop through the file until we find the exact ID
    while (fread(&current_record, sizeof(Record), 1, file) == 1) {
        if (current_record.id == target_id) {
            found = 1;
            
            // Format the timestamp
            char record_time[64];
            time_t timestamp = (time_t)current_record.timestamp;
            struct tm *rec_tm_info = localtime(&timestamp);
            strftime(record_time, sizeof(record_time), "%Y-%m-%d %H:%M:%S", rec_tm_info);

            // Print the FULL data
            printf(STYLE_BOLD COLOR_CYAN "\n--- Full Report Details ---\n" COLOR_RESET);
            printf(COLOR_YELLOW "Report ID    : %d\n" COLOR_RESET, current_record.id);
            // Using a ternary operator to print "N/A" if the string is empty
            printf("Inspector    : %s\n", current_record.inspector_name[0] != '\0' ? current_record.inspector_name : "N/A");
            printf("Category     : %s\n", current_record.issue_category);
            printf("Security Lvl : %d\n", current_record.security_level);
            printf("Location     : Lat %.4f, Lon %.4f\n", current_record.latitude, current_record.longitude);
            printf("Logged At    : %s\n", record_time);
            printf("Description  : %s\n", current_record.description[0] != '\0' ? current_record.description : "N/A");
            printf("---------------------------\n\n");
            
            break; // Stop searching once we found it
        }
    }

    if (!found) {
        printf(COLOR_YELLOW "  [WARNING] " COLOR_RESET "Report ID %d not found in %s.\n", target_id, district_id);
    }

    fclose(file);
}