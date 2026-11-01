/**
 * @file util.h
 * @brief Utility macros for logging, timing, and colored terminal output.
 */

#ifndef UTIL_H
#define UTIL_H

#include "conf.h"

#include <linux/limits.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define DATE_MAX 128

#ifndef _WIN32
#define ERROR_COLOR "\e[1;31m"  ///< ANSI red color for error messages.
#define DEBUG_COLOR "\e[1;34m"  ///< ANSI blue color for debug messages.
#define RESET_COLOR "\e[0m"     ///< ANSI reset sequence.
#else
#define ERROR_COLOR ""          ///< Empty on Windows.
#define DEBUG_COLOR ""          ///< Empty on Windows.
#define RESET_COLOR ""          ///< Empty on Windows.
#endif

/**
 * @def DEBUG
 * @brief Prints a debug message to stdout when @c DEBUG_MODE is enabled.
 * @details Output is suppressed entirely when @c DEBUG_MODE is 0.
 * @param fmt printf-style format string.
 * @param ... Optional format arguments.
 */
#define DEBUG(fmt, ...)                                             \
do {                                                                \
    if (DEBUG_MODE) {                                               \
        fprintf(stderr, "%sDEBUG:%s ", DEBUG_COLOR, RESET_COLOR);   \
        fprintf(stderr, fmt, ##__VA_ARGS__);                        \
        fprintf(stderr, "\n");                                      \
    }                                                               \
} while (0)

/**
 * @def ERROR
 * @brief Prints an error message to stderr.
 * @param fmt printf-style format string.
 * @param ... Optional format arguments.
 */
#define ERROR(fmt, ...)                                         \
do {                                                            \
    fprintf(stderr, "%sERROR:%s ", ERROR_COLOR, RESET_COLOR);   \
    fprintf(stderr, fmt, ##__VA_ARGS__);                        \
    fprintf(stderr, "\n");                                      \
} while (0)

#define PTR_RECIEVE_FAIL_VOID(ptr, func)                \
do {                                                    \
    if ((ptr) == NULL) {                                \
        ERROR("%s Failed: %s is NULL", #func, #ptr);    \
        return;                                         \
    }                                                   \
} while(0)

#define PTR_RECIEVE_FAIL_PTR(ptr, func)                 \
do {                                                    \
    if ((ptr) == NULL) {                                \
        ERROR("%s Failed: %s is NULL", #func, #ptr);    \
        return NULL;                                    \
    }                                                   \
} while(0)

#define FREE(...)                                                   \
do {                                                                \
    void *ptrs[] = { __VA_ARGS__ };                                 \
    for (size_t i = 0; i < sizeof(ptrs) / sizeof(ptrs[0]); ++i) {  \
        if (ptrs[i] != NULL) free(ptrs[i]);                         \
    }                                                               \
} while(0)

#define ALLOC_FAIL(ptr)                         \
do {                                            \
    if ((ptr) == NULL) {                        \
        ERROR("Failed to allocate: %s", #ptr);  \
        return NULL;                            \
    }                                           \
} while (0)

bool
verify_path(const char* root, const char* name, unsigned char type);

bool
create_subdir(const char* root, const char* name);

bool
get_log_file(const char* path, const char* sub, char *buffer);

bool
get_bin_file(const char* path, const char* sub, const char* hashed, char* buffer);

char*
mask(const char* str, char mask, size_t keep);

char*
fuzz(const char* input, int x);

bool
verify_file(const char* path);

#define SIZE 65

#ifndef LIB
#define LIB ""
#endif

typedef void (*handler)(char*, char[SIZE]);

handler
get_handler(void **handle);

#endif
