#include "account.h"
#include "util.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

enum mode
mode_request()
{
    fprintf(stdout, "Choose mode (0-create/1-login): ");
    char* l = sh_get_input();
    if (l == NULL) {
        ERROR("failed to request input");
        return LOGIN;
    }
    if (strcmp(l, "1") == 0) {
        return LOGIN;
    } else return CREATE;
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

#define LOGLEN 4

// login max=4bytes, pass max=8bytes, sum=12bytes
void
account_create(struct pair* p)
{
    // TODO: check if account already created
    PTR_RECIEVE_FAIL_VOID(p, account_create);
    size_t loglen = strlen(p->login);
    if (loglen > 4) {
        ERROR("login is too long");
        return;
    }

    if (fbin != NULL) {
        unsigned char logbuf[LOGLEN];
        clearerr(fbin);
        fseek(fbin, 0, SEEK_SET);
        while (fread(logbuf, sizeof(char), LOGLEN, fbin) == LOGLEN) {
            if (memcmp(logbuf, p->login, LOGLEN) == 0) {
                ERROR("account already exists");
                return;
            }
        }

        fseek(fbin, 0, SEEK_END);
    }

    uint64_t hashed = hash(p->password);

    char logbuf[LOGLEN] = {0};
    memcpy(logbuf, p->login, loglen);

    WRITEBIN(logbuf, sizeof(char), LOGLEN);
    WRITEBIN(&hashed, sizeof(uint64_t), 1);
    fflush(fbin);
    LOG("new account created\tlogin=%s\tpass=%lu", p->login, hashed);
}
