#include "account.h"
#include "util.h"

#include <stdlib.h>

char*
sh_get_input()
{
    size_t cap = 10;
    size_t i = 0;
    char* str = (char*)malloc(cap);
    char c;
    while (scanf("%c", &c) == 1 && c != '\n') {
        if (i + 1 >= cap) {
            cap += 10;
            char* tmp = (char*)realloc(str, cap);
            if (tmp == NULL) {
                free(str);
                return NULL;
            }
            str = tmp;
        }
        str[i] = c;
        i++;
    }

    if (i == 0 && feof(stdin)) {
        free(str);
        return NULL;
    }
    str[i] = '\0';
    return str;
}

char*
login_request(void)
{
    fprintf(stdout, "Input your account login: ");
    char* l = sh_get_input();
    if (l == NULL) {
        ERROR("error recieving input");
        return NULL;
    }
    return l;
}

char*
password_request(void)
{
    fprintf(stdout, "Input your password: ");
    char* p = sh_get_input();
    if (p == NULL) {
        ERROR("error recieving input");
        return NULL;
    }
    return p;
}

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
    LOG("new account created\tlogin=%s\tpass=%lu", p->login, hashed);
}
