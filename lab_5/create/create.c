#include "util.h"
#include "create.h"

#include <stdlib.h>

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
    if (p != NULL) {
        free(p);
    }
}

void
pair_encrypt(struct pair* p)
{
    PTR_RECIEVE_FAIL_VOID(p, pair_encrypt);

}

bool
pair_write(struct pair* p)
{
    if (p == NULL) {
        ERROR("error revieving pair pointer to write");
        return false;
    }
    return false;
}
