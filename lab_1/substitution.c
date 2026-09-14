#include "substitution.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

buffer*
buffer_alloc(const wchar_t *str)
{
    DEBUG("Call buffer_alloc");
    if (str == NULL) {
        ERROR("Failed to allocate buffer from string %p", str);
        return NULL;
    }
    buffer* result;
    result = (buffer*)malloc(sizeof(buffer));
    ALLOC_FAIL(result);
    size_t len = wcslen(str);
    result->cypher = NULL;
    result->raw = (wchar_t*)malloc((len+1)*sizeof(wchar_t));
    if (result->raw == NULL) {
        ERROR("Failed to allocate result->raw");
        free(result);
        return NULL;
    }
    wcscpy(result->raw, str);
    result->size = len;
    result->N = 0;
    return result;
}

void
buffer_free(buffer* ptr)
{
    DEBUG("Call buffer_free");
    FREE_FAIL(ptr);
    free(ptr->raw);
    free(ptr->cypher);
    free(ptr);
}

void
build_alphabet(buffer* buf, const wchar_t* given)
{
    DEBUG("Call build_alphabet");
    PTR_RECIEVE_FAIL_VOID(buf, build_alphabet);
    PTR_RECIEVE_FAIL_VOID(given, build_alphabet);
    buf->N = 0;
    memset(buf->alphabet, 0, sizeof(buf->alphabet));
    for (int i = 0; given[i] != '\0'; ++i) {
        wchar_t c = given[i];
        if (wcsrchr(LETTER, c) != NULL ||
            wcschr(DIGIT, c) != NULL ||
            wcschr(PUNCTUATION, c) != NULL) {
            if (wcschr(buf->alphabet, c) == NULL) {
                if (buf->N < ALPHABET_SIZE - 1) {
                    buf->alphabet[buf->N++] = c;
                }
            }
        }
    }
}
