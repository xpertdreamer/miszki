#include "util.h"
#include <dlfcn.h>
#include <stdio.h>
#include <sys/stat.h>

bool
get_bin_file(const char* path, const char* sub, const char* hashed, char* buffer)
{
    if (path == NULL || sub == NULL || hashed == NULL) {
        ERROR("failed to recieve pointers [get_bin_file]");
        return false;
    }

    int len = snprintf(buffer, PATH_MAX, "%s/%s/%s", path, sub, hashed);
    if (len < 0 || len > (int)PATH_MAX) {
        ERROR("failed to snprintf binary path");
        return false;
    }
    buffer[len] = '\0';
    DEBUG("bin path:%s", buffer);

    return true;
}

bool
verify_file(const char* path) {
    if (path == NULL) {
        ERROR("failed to recieve pointers [verify_file]");
        return false;
    }
    struct stat st;
    return (stat(path, &st) == 0);
}
