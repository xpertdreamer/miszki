#include "account.h"
#include "util.h"

#include <stdlib.h>

struct pair*
pair_create(char* login, char* pass)
{
    PTR_RECIEVE_FAIL_PTR(login, pair_create);
    PTR_RECIEVE_FAIL_PTR(pass, pair_create);
    struct pair* p = (struct pair*)malloc(sizeof(struct pair));
    ALLOC_FAIL(p);
    p->login = login;
    p->password = pass;
    return p;
}

void
account_create(struct pair* p)
{
    PTR_RECIEVE_FAIL_VOID(p, account_create);
    unsigned long hashed = hash(p->password);
    // TODO: save account details into file
}
