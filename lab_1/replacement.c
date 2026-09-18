#include "replacement.h"
#include "util.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define INITIAL_FROM L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя0123456789"
#define INITIAL_TO   L"9876543210яюэьыъщшчцхфутсрпонмлкйизжёедгвба"

#define DELIMITER L"\r\n"

static replace_table*
build_initial()
{
    replace_table* initial = (replace_table*)malloc(sizeof(replace_table));
    ALLOC_FAIL(initial);
    initial->count = wcslen(INITIAL_FROM);
    initial->entries = (entry*)malloc(initial->count * sizeof(entry));
    if (initial->entries == NULL) {
        ERROR("Failed to alocate initial entries");
        free(initial);
        return NULL;
    }
    for (size_t i = 0; i < initial->count; ++i) {
        initial->entries[i].from = INITIAL_FROM[i];
        initial->entries[i].to = INITIAL_TO[i];
    }
    return initial;
}

static size_t
entry_size(wchar_t* buffer)
{
    DEBUG("Call entry_size");
    wchar_t* copy = wcsdup(buffer);
    if (copy == NULL) {
        ERROR("Failed to create copy of buffer");
        return 0;
    }
    size_t count = 0;
    wchar_t *token, *state;
    for (token = wcstok(copy, DELIMITER, &state); token != NULL; token = wcstok(NULL, DELIMITER, &state)) {
        wchar_t dummy_f, dummy_t;
        if (swscanf(token, L"[%c:%c]", &dummy_f, &dummy_t) == 2) ++count;
    }
    free(copy);
    return count;
}

replace_table*
build_table(const char* path)
{
    DEBUG("Call build_table");
    replace_table* result = NULL;
    if (path == NULL) {
        DEBUG("No path to table of replacement providen : fallback initial table");
        result = build_initial();
        PTR_RECIEVE_FAIL_PTR(result, build_table);
        return result;
    }
    DEBUG("Reading replace table from file");
    wchar_t* buf = read_file(path);
    PTR_RECIEVE_FAIL_PTR(buf, build_table);
    size_t entry_len = entry_size(buf);
    if (entry_len == 0) {
        ERROR("Failed to create table : no tokens providen");
        free(buf);
        return NULL;
    }
    result = (replace_table*)malloc(sizeof(replace_table));
    if (result == NULL) {
        ERROR("Failed to allocate table");
        free(buf);
        return NULL;
    }
    result->count = entry_len;
    result->entries = (entry*)malloc(result->count * sizeof(entry));
    if (result->entries == NULL) {
        ERROR("Failed to alocate initial entries");
        free(result);
        free(buf);
        return NULL;
    }
    wchar_t *token, *state;
    size_t i = 0;
    for (token = wcstok(buf, DELIMITER, &state); token != NULL; token = wcstok(NULL, DELIMITER, &state)) {
        if (i >= result->count) {
            DEBUG("Warning: found more valid tokens than expected");
            break;
        }
        entry* curr = &result->entries[i];
        if (swscanf(token, L"[%lc:%lc]", &curr->from, &curr->to) == 2) ++i;
        else DEBUG("Token is invalid");
    }
    result->count = i;
    free(buf);
    return result;
}

void
r_encrypt(replace_table* table, replacement_buffer* buffer)
{
    DEBUG("Call encrypt");
    PTR_RECIEVE_FAIL_VOID(table, encrypt);
    PTR_RECIEVE_FAIL_VOID(buffer, encrypt);
    size_t len = buffer->size, i = 0;
    do {
        wchar_t current = buffer->raw[i];
        bool found = false;
        size_t j = 0;
        for (; j < table->count; ++j) {
            if (table->entries[j].from == current) {
                found = true;
                break;
            }
        }
        if (found == true) buffer->processed[i] = table->entries[j].to;
        else buffer->processed[i] = buffer->raw[i];
        i++;
    } while (i < len);
    buffer->processed[i] = L'\0';
}

void
r_decrypt(replace_table* table, replacement_buffer* buffer)
{
    DEBUG("Call decrypt");
    PTR_RECIEVE_FAIL_VOID(table, decrypt);
    PTR_RECIEVE_FAIL_VOID(buffer, decrypt);
    size_t len = buffer->size, i = 0;
    do {
        wchar_t current = buffer->raw[i];
        bool found = false;
        size_t j = 0;
        for (; j < table->count; ++j) {
            if (table->entries[j].to == current) {
                found = true;
                break;
            }
        }
        if (found == true) buffer->processed[i] = table->entries[j].from;
        else buffer->processed[i] = buffer->raw[i];
        i++;
    } while (i < len);
    buffer->processed[i] = L'\0';
}

void
free_workspace(replace_table* table, replacement_buffer* buf)
{
    DEBUG("Call free_workspace");
    FREE_FAIL(table);
    if (table->entries != NULL) free(table->entries);
    free(table);
    FREE_FAIL(buf);
    if (buf->processed != NULL) free(buf->processed);
    if (buf->raw != NULL) free(buf->raw);
    free(buf);
}

replacement_buffer*
replace_buf_alloc(const wchar_t* str)
{
    DEBUG("Call replace_buffer_alloc");
    if (str == NULL) {
        ERROR("Failed to allocate buffer from string %p", str);
        return NULL;
    }
    replacement_buffer* result;
    result = (replacement_buffer*)malloc(sizeof(replacement_buffer));
    ALLOC_FAIL(result);
    size_t len = wcslen(str);
    result->processed = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    if (result->processed == NULL) {
        ERROR("Failed to allocate: cypher buffer");
        free(result);
        return NULL;
    }
    result->raw = (wchar_t*)malloc((len+1)*sizeof(wchar_t));
    if (result->raw == NULL) {
        ERROR("Failed to allocate result->raw");
        free(result->processed);
        free(result);
        return NULL;
    }
    wcscpy(result->raw, str);
    result->size = len;
    return result;
}

void
save_replace_table(replace_table* ptr, const char* path)
{
    DEBUG("Call save_replace_table");
    PTR_RECIEVE_FAIL_VOID(ptr, save_replace_table);
    size_t len = sizeof(wchar_t)*(ptr->count*2+1);
    wchar_t* wstring = (wchar_t*)malloc(len);
    if (wstring == NULL) {
        ERROR("Failed to allocate wstring");
        return;
    }
    for (size_t i = 0; i < ptr->count; ++i) {
        wstring[i*2] = ptr->entries[i].from;
        wstring[i*2+1] = ptr->entries[i].to;
    }
    wstring[ptr->count*2] = L'\0';
    write_file(path, wstring);
    FREE_WCHAR(wstring);
    return;
}
