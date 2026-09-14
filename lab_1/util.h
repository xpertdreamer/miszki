/**
 * @file util.h
 * @brief Utility macros for logging, timing, and colored terminal output.
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#include "conf.h"

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
#define DEBUG(fmt, ...)                                                 \
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
#define ERROR(fmt, ...)                                                 \
    do {                                                                \
        fprintf(stderr, "%sERROR:%s ", ERROR_COLOR, RESET_COLOR);       \
        fprintf(stderr, fmt, ##__VA_ARGS__);                            \
        fprintf(stderr, "\n");                                      \
    } while (0)

#define ALLOC_FAIL(ptr)                             \
    do {                                            \
        if ((ptr) == NULL) {                        \
            ERROR("Failed to allocate: %s", #ptr);  \
            return NULL;                            \
        }                                           \
    } while (0)

#define FREE_FAIL(ptr)                          \
    do {                                        \
        if ((ptr) == NULL) {                    \
            ERROR("Failed to free: %s", #ptr);  \
            return;                             \
        }                                       \
    } while(0)

#define PTR_RECIEVE_FAIL_VOID(ptr, func)                    \
    do {                                                    \
        if ((ptr) == NULL) {                                \
            ERROR("%s Failed: %s is NULL", #func, #ptr);    \
            return;                                         \
        }                                                   \
    } while(0)

#define PRINT_WCHAR(str) do { fflush(stdout); wprintf(L"%ls\n", (str)); fflush(stdout); } while(0)

#define FREE_WCHAR(ptr) do { if ((ptr) != NULL) free((ptr)); }while(0)

#endif
