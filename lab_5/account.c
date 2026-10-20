#include "account.h"
#include "util.h"

#include <stdlib.h>

struct pair*
pair_create(char* login, char* pass)
{
    PTR_RECIEVE_FAIL_PTR(login, pair_create);
    PTR_RECIEVE_FAIL_PTR(pass, pair_create);
    struct pair* p = (struct pair*)malloc(sizeof(struct pair));
    p->login = login;
    p->password = pass;
    return p;
}
