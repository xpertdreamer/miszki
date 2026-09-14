#include "input.h"
#include "util.h"

#include <string.h>
#include <stdlib.h>

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
