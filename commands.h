#ifndef MY_UTILS_H
#define MY_UTILS_H

#include <stddef.h> // Required for size_t

// --- Struct Definitions ---
typedef struct {
    int id;
    char inspector_name[50];
    float latitude;
    float longitude;
    char issue_category[50];
    int security_level;
    size_t timestamp;
    char description[256];
} Record;

// --- Function Prototypes ---
void make_directory_with_cfg(const char* filepath, const char* dir_name);
void handle_add(const char *dir_name, const char *file_name);
void handle_remove(const char *filename);
void handle_help(void);
void handle_list(const char *district_id);
void handle_view(const char *district_id, const char *report_id);
void spinner_loading(void);
Record add_record_file(void);
void add_record(const char* filename, Record* record);

// --- Color Macros ---
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"

// --- Text Style Macros ---
#define STYLE_BOLD    "\033[1m"
#define STYLE_DIM     "\033[2m"

#endif  