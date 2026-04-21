#ifndef MY_UTILS_H
#define MY_UTILS_H

void make_directory_with_cfg(const char* filepath, const char* dir_name);
void handle_add(const char *dir_name, const char *file_name);
void handle_remove(const char *filename);
void handle_help(void);

void spinner_loading(void);


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