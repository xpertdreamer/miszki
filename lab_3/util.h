/**
 * @file util.h
 * @brief Utility macros for logging, timing, and colored terminal output.
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <chrono>
#include <iostream>

#include "conf.h"

#ifndef _WIN32
#define ERROR_COLOR "\e[1;31m"  ///< ANSI red color for error messages.
#define DEBUG_COLOR "\e[1;34m"  ///< ANSI blue color for debug messages.
#define TODO_COLOR "\e[0;33m"   ///< ANSI yellow color for todo messages.
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

#define PTR_RECIEVE_FAIL_PTR(ptr, func)                     \
    do {                                                    \
        if ((ptr) == NULL) {                                \
            ERROR("%s Failed: %s is NULL", #func, #ptr);    \
            return NULL;                                    \
        }                                                   \
    } while(0)

#define TODO(text)                                                      \
    do {                                                                \
        fprintf(stderr, "%sTODO:%s %s\n", TODO_COLOR, RESET_COLOR, (text)); \
    } while(0)


/**
 * @def START
 * @brief Starts a timer with the given name.
 * @param name Identifier for the timer.
 */
#define START(name)                                                            \
  auto start_##name = std::chrono::high_resolution_clock::now();

/**
 * @def END
 * @brief Stops the timer and prints the elapsed time in microseconds.
 * @param name Identifier matching the corresponding @ref START call.
 */
#define END(name) \
    auto end_##name = std::chrono::high_resolution_clock::now();        \
    auto duration_##name = std::chrono::duration_cast<std::chrono::microseconds>(end_##name - start_##name).count(); \
    std::cout << "Timer [" << #name << "] took: " << duration_##name << " microseconds\n";

#endif
