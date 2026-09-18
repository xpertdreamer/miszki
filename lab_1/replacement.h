#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include <stddef.h>

typedef struct {
    wchar_t from;
    wchar_t to;
} entry;

typedef struct {
    entry* entries;
    size_t count;
} replace_table;

typedef struct {
    size_t size;
    wchar_t* raw;
    wchar_t* processed;
} replacement_buffer;

replace_table*
build_table(const char* path);

void
save_replace_table(replace_table* ptr, const char* path);

void
free_workspace(replace_table* table, replacement_buffer* buf);
/* replace_table* */
/* build_replace_table(args); */

#endif
