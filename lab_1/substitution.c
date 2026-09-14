#include "substitution.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>

buffer*
buffer_alloc(const char *str)
{
    DEBUG("Call buffer_alloc");
    if (str == NULL) {
        ERROR("Failed to allocate buffer from string %p", str);
        return NULL;
    }
    buffer* result;
    result = (buffer*)malloc(sizeof(buffer));
    ALLOC_FAIL(result);
    size_t len = strlen(str);
    result->cypher = NULL;
    result->raw = (char*)malloc((len+1)*sizeof(char));
    if (result->raw == NULL) {
        ERROR("Failed to allocate result->raw");
        free(result);
        return NULL;
    }
    strcpy(result->raw, str);
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
build_alphabet(buffer* buf, const char* given)
{
    DEBUG("Call build_alphabet");
    PTR_RECIEVE_FAIL_VOID(buf, build_alphabet);
    PTR_RECIEVE_FAIL_VOID(given, build_alphabet);
    buf->N = 0;
    memset(buf->alphabet, 0, ALPHABET_SIZE);
    for (int i = 0; given[i] != '\0'; ++i)
        if (have(buf->alphabet, buf->N, LETTER[i]) == true) buf->alphabet[buf->N++] = LETTER[i];
    for (int i = 0; given[i] != '\0'; ++i)
        if (have(buf->alphabet, buf->N, DIGIT[i]) == true) buf->alphabet[buf->N++] = DIGIT[i];
    for (int i = 0; given[i] != '\0'; ++i)
        if (have(buf->alphabet, buf->N, PUNCTUATION[i]) == true) buf->alphabet[buf->N++] = PUNCTUATION[i];
}
