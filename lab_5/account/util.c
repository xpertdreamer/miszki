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
get_log_file(const char* path, const char* sub, char *buffer)
{
    if (path == NULL || sub == NULL) {
        ERROR("failed to recieve pointers [get_log_file]");
        return false;
    }

    time_t rawtime;
    struct tm* timeinfo;
    char temp[DATE_MAX];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(temp, DATE_MAX, "%d-%m-%Y_%H:%M:%S",timeinfo);

    int len = snprintf(buffer, PATH_MAX, "%s/%s/%s_%s.log", path, sub, temp, EXECUTABLE_NAME);
    if (len < 0 || len > (int)PATH_MAX) {
        ERROR("failed to snprintf full path = %s + %s + %s", path, sub, temp);
        return false;
    }
    buffer[len] = '\0';
    DEBUG("log path:%s", buffer);

    return true;
}

bool
create_subdir(const char* root, const char* name)
{
    if (root == NULL || name == NULL) {
        ERROR("failed to recieve pointers [create_subdir]");
        return false;
    }

    char target[PATH_MAX];
    int len = snprintf(target, sizeof(target), "%s/%s", root, name);
    if (len < 0 || len > (int)sizeof(target)) {
        ERROR("failed to snprintf full path = %s + %s", root, name);
        return false;
    }
    target[len] = '\0';

    int status = mkdir(target, 0755);
    return status == 0 ? true : false;
}

bool
verify_path(const char* root, const char* name, unsigned char type)
{
    if (root == NULL || name == NULL) {
        ERROR("failed to recieve pointers [verify_path]");
        return false;
    }
    DIR *dir = opendir(root);
    if (!dir) {
        ERROR("%d cannot open root (%s)", errno, strerror(errno));
        return false;
    }
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == type && (strcmp(ent->d_name, name) == 0)) {
            closedir(dir);
            return true;
        }
    }
    ERROR("cannot find given path");
    closedir(dir);
    return false;
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

handler
get_handler(void **handle) {
    void (*enc)(char*, char[SIZE]);
    char* err;

    handle = dlopen(LIB, RTLD_NOW);
    if (handle == NULL) {
        ERROR("failed to load encryption lib: %s", dlerror());
        return NULL;
    }
    dlerror();

    enc = (void (*)(char*, char[SIZE])) dlsym(handle, "encrypt");
    err = dlerror();
    if (err != NULL) {
        ERROR("%s", err);
        dlclose(handle);
        return NULL;
    }

    return enc;
}

char*
mask(const char* str, char mask, size_t keep)
{
    size_t len = strlen(str);
    if (len == 0) return NULL;
    char *output = (char*)malloc(len + 1);
    ALLOC_FAIL(output);
    strncpy(output, str, len);
    memset(output, mask, len - keep);
    output[len] = '\0';
    return output;
}
