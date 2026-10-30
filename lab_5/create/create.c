#include "util.h"
#include "create.h"

#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

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

bool
pair_encrypt(struct pair* p)
{
    if (p == NULL || p->login == NULL || p->password == NULL) {
        ERROR("error revieving pair pointer to encrypt");
        return false;
    }
    void *handle;
    void (*enc)(char*, char[SIZE]);
    char* err;

    handle = dlopen(LIB, RTLD_NOW);
    if (handle == NULL) {
        ERROR("failed to load encryption lib: %s", dlerror());
        return false;
    }
    dlerror();

    enc = (void (*)(char*, char[SIZE])) dlsym(handle, "encrypt");
    err = dlerror();
    if (err != NULL) {
        ERROR("%s", err);
        dlclose(handle);
        return false;
    }

    char pass_buf[SIZE];
    enc(p->password, pass_buf);

    free(p->password);
    p->password = NULL;
    p->password = strdup(pass_buf);
    if (p->password == NULL) {
        ERROR("failed to allocate hashed memory");
        dlclose(handle);
        return false;
    }

    dlclose(handle);
    return true;
}

// NOTE: use openssl to generate hex-number
bool
pair_write(struct pair* p)
{
    if (p == NULL) {
        ERROR("error revieving pair pointer to write");
        return false;
    }
    return false;
}
