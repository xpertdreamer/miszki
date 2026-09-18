#include "input.h"
#include "util.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

mode
match_mode(const char* arg)
{
    DEBUG("Call match_mode");
    if (arg == NULL) {
        ERROR("match_mode: recieved arg is NULL : fallback encrypt");
        return encryption;
    }
    if (strcmp(arg, ENC_MODE) == 0) return encryption;
    else if (strcmp(arg, DEC_MODE) == 0) return decryption;
    ERROR("Providen mode is not supported : fallback encrypt");
    return encryption;
}

wchar_t*
char_to_wchar(const char *c)
{
    DEBUG("Call char_to_wchar");
    if (c == NULL) {
        ERROR("Given string pointer is NULL");
        return NULL;
    }
    size_t len = mbstowcs(NULL, c, 0);
    if (len == (size_t)-1) {
        ERROR("Invalid multibyte sequence");
        return NULL;
    }
    wchar_t *wc = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    if (wc == NULL) {
        ERROR("wc allocation failed");
        return NULL;
    }
    mbstowcs(wc, c, len + 1);
    return wc;
}

static size_t
regex_size(const wchar_t* range, size_t range_len) {
    size_t result = 0;
    for (size_t i = 1; i < range_len - 1; ++i) {
        if (range[i] == L'-' && i > 1 && i < range_len - 2) {
            wchar_t first = range[i-1];
            wchar_t second = range[i+1];
            if (first <= second) {
                result += (second - first);
            }
            ++i;
            continue;
        }
        ++result;
    }
    return result + 1;
}

wchar_t*
regex_to_wchar(const wchar_t* range)
{
    DEBUG("Call regex_to_char");
    wchar_t* result;
    PTR_RECIEVE_FAIL_PTR(range, regex_to_char);
    size_t range_len = wcslen(range);
    if (range[0] != L'[' || range[range_len - 1] != L']') {
        ERROR("Regex format is incorrect");
        return NULL;
    }
    size_t result_len = regex_size(range, range_len);
    result = (wchar_t*)malloc(result_len * sizeof(wchar_t));
    ALLOC_FAIL(result);
    wchar_t* write = result;
    for (size_t i = 1; i < range_len - 1; ++i) {
        if (range[i] == L'-' && i > 1 && i < range_len - 2) {
            wchar_t first = range[i-1];
            wchar_t second = range[i+1];
            if (first <= second) {
                for (wchar_t wc = first + 1; wc <= second; wc++) {
                    *write = wc;
                    write++;
                }
            }
            ++i;
            continue;
        }
        *write = range[i];
        ++write;
    }
    *write = L'\0';
    return result;
}

wchar_t*
read_file(const char* path)
{
    FILE *fptr;
    char* buffer = NULL;
    size_t len;
    fptr = fopen(path, "rb");
    if (fptr == NULL) {
        ERROR("Failed to open file");
        return NULL;
    }
    if (fseek(fptr, 0L, SEEK_END) != 0) {
        ERROR("fseek error");
        fclose(fptr);
        return NULL;
    }
    len = ftell(fptr);
    rewind(fptr);
    buffer = (char*)malloc(len+1);
    if (buffer == NULL) {
        ERROR("Failed to allocate file buffer");
        fclose(fptr);
        return NULL;
    }
    fread(buffer, 1, len, fptr);
    fclose(fptr);
    buffer[len] = '\0';
    wchar_t* res = char_to_wchar(buffer);
    free(buffer);
    return res;
}

bool
write_file(const char* path, const wchar_t* wstring)
{
    DEBUG("Call write_file");
    FILE* fptr = fopen(path, "w");
    if (fptr == NULL) {
        ERROR("Failed to open file to write");
        return false;
    }
    size_t len = wcslen(wstring);
    for (size_t i = 0; i + 1 < len; i+=2) fwprintf(fptr, L"[%lc:%lc]\n", wstring[i], wstring[i+1]);
    fclose(fptr);
    return true;
}
