#include "conf.h"
#include "util.h"
#include "create.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

struct pair*
pair_alloc(char* login, char* pass)
{
    PTR_RECIEVE_FAIL_PTR(login, pair_alloc);
    PTR_RECIEVE_FAIL_PTR(pass, pair_alloc);
    struct pair* pair_result = (struct pair*)malloc(sizeof(struct pair));
    ALLOC_FAIL(pair_result);
    pair_result->login = login;
    pair_result->password = pass;
    return pair_result;
}

void
pair_dealloc(struct pair* p)
{
    if (p == NULL) return;
    FREE(p->password, p->login);
    FREE(p);
}

typedef void (*handler)(char*, char[SIZE]);

handler
get_handler(void** handle) {
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

bool
pair_encrypt(struct pair* p)
{
    if (p == NULL || p->login == NULL || p->password == NULL) {
        ERROR("error revieving pair pointer to encrypt");
        return false;
    }

    void *handle = NULL;
    handler enc = get_handler(&handle);
    if (enc == NULL) {
        ERROR("cannot get handler");
        if (handle != NULL) dlclose(handle);
        return false;
    }
    char pass_buf[SIZE];
    enc(p->password, pass_buf);

    free(p->password);
    p->password = NULL;
    p->password = strdup(pass_buf);
    if (p->password == NULL) {
        ERROR("failed to allocate hashed memory");
        if (handle != NULL) dlclose(handle);
        return false;
    }

    if (handle != NULL) dlclose(handle);
    return true;
}

#define PH_SIZE 11
#define CN_SIZE 16
#define CV_SIZE 3

// NOTE: use openssl to generate hex-number
bool
pair_write(struct pair* p, const char* path)
{
    if (p == NULL || path == NULL || p->login == NULL || p->password == NULL) {
        ERROR("error revieving pair or path pointer to write");
        return false;
    }

    void *handle;
    handler enc = get_handler(&handle);
    if (enc == NULL) {
        ERROR("cannot get handler");
        if (handle != NULL) dlclose(handle);
        return false;
    }

    char login_buf[SIZE];
    char temp[PATH_MAX];
    enc(p->login, login_buf);
    if (get_bin_file(path, DB_SUBDIR, login_buf, temp) == false) {
        ERROR("failed to get binary path");
        if (handle != NULL) dlclose(handle);
        return false;
    }

    if (verify_file(temp)) {
        ERROR("this account already exists");
        if (handle != NULL) dlclose(handle);
        return false;
    }

    // TODO: replace with syscall
    FILE *fptr = fopen(temp, "wb");
    if (fptr == NULL) {
        ERROR("cannot open db file to write");
        if (handle != NULL) dlclose(handle);
        return false;
    }

    char initial_data[PH_SIZE+CN_SIZE+CV_SIZE] = {0};
    fwrite(p->password, 1, SHA256_LEN, fptr);
    fwrite(initial_data, sizeof(char), PH_SIZE+CN_SIZE+CV_SIZE, fptr);


    fclose(fptr);
    if (handle != NULL) dlclose(handle);
    return true;
}
