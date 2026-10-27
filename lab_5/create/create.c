#include "util.h"
#include "create.h"

#include <stdio.h>
#include <stdlib.h>

struct pair*
pair_alloc(char* login, char* pass)
{
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
