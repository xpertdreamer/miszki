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
    return result;
}

void
buffer_free(buffer* ptr)
{

}
