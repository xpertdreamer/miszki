/**
 * @file util.h
 * @brief Utility macros for logging, timing, and colored terminal output.
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

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

#define MAX_PATH_LEN 1024

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

extern FILE *flog;

#define LOG(fmt, ...)                               \
    do {                                            \
    if ((flog) != NULL) {                           \
        time_t rawtime;                             \
        time(&rawtime);                             \
        struct tm* timeinfo = localtime(&rawtime); \
        char* time_str = asctime(timeinfo); \
        size_t len = strlen(time_str);              \
        if (len > 0 && time_str[len-1] == '\n') {\
            time_str[len - 1] = '\0';                \
        } \
        fprintf(flog, "[%s] ", time_str);   \
                fprintf(flog, fmt, ##__VA_ARGS__);  \
                fprintf(flog, "\n");                \
        fflush(flog);                               \
    }                                               \
    } while(0)

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

#define PTR_RECIEVE_FAIL_PTR(ptr, func)                     \
    do {                                                    \
        if ((ptr) == NULL) {                                \
            ERROR("%s Failed: %s is NULL", #func, #ptr);    \
            return NULL;                                    \
        }                                                   \
        } while(0)

inline void
get_path(char** argv, char* buffer, const char* name)
{
    if (argv == NULL) {
        ERROR("cannot get argv");
        return;
    }
    char buf[MAX_PATH_LEN];
    char* realp = realpath(argv[0], buf);
    if (!realp) {
        ERROR("cannot get real path");
        exit(EXIT_FAILURE);
    }
    buf[strlen(buf) - 4] = '\0';
    snprintf(buffer, MAX_PATH_LEN, "%.1000s%s", buf, name);
}

inline unsigned long
hash(char* str) // djb2
{
    unsigned long h = 5381;
    int c;
    while ((c = *str++)) {
        h = ((h << 5) + h) + c;
    }
    return h;
}

/* #define MASK(str, type)                         \ */
/*     do {                                        \ */
/*         // TODO: mask value of str depending on type \ */
/*     } while(0) */

#endif
