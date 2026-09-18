#include "replacement.h"
#include "util.h"
#include "input.h"

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
        if (swscanf(token, L"[%c:%c]", &curr->from, &curr->to) == 2) ++i;
        else DEBUG("Token is invalid");
    }
    result->count = i;
    free(buf);
    return result;
}

void
encrypt(replace_table* table, replacement_buffer* buffer)
{
    PTR_RECIEVE_FAIL_VOID(table, encrypt);
    PTR_RECIEVE_FAIL_VOID(buffer, encrypt);
    size_t len = wcslen(buffer->raw), i = 0;
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
