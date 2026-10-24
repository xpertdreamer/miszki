#include "account.h"
#include "util.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>

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
        free(l);
        return LOGIN;
    }
    if (strcmp(l, "1") == 0) {
        free(l);
        return LOGIN;
    } else { free(l); return CREATE; }
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
pair_destroy(struct pair* p)
{
    if (p != NULL) {
        free(p);
    }
}

void
account_destroy(struct account* a)
{
    if (a != NULL) {
        free(a);
    }
}

// login max=4bytes, pass max=8bytes, sum=12bytes
void
account_create(struct pair* p)
{
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

struct account*
account_login(struct pair* p)
{
    PTR_RECIEVE_FAIL_PTR(p, account_login);
    size_t loglen = strlen(p->login);
    if (loglen > 4 || loglen == 0) {
        ERROR("login is too long or empty");
        return NULL;
    }
    if (fbin == NULL) {
        ERROR("data corruption");
        return NULL;
    }

    unsigned char logbuf[LOGLEN];
    uint64_t dummy;
    bool found = false;
    fseek(fbin, 0, SEEK_SET);

    while (fread(logbuf, sizeof(char), LOGLEN, fbin) == LOGLEN) {
        if (fread(&dummy, sizeof(uint64_t), 1, fbin) != 1) {
            ERROR("data corruption");
            return NULL;
        }
        if (strncmp((const char*)logbuf, p->login, LOGLEN) == 0) {
            found = true;
            break;
        }
    }

    if (found == false) {
        ERROR("account not found");
        return NULL;
    }
    if (dummy != hash(p->password)) {
        ERROR("password is incorrect");
        return NULL;
    }

    fseek(fbin, 0, SEEK_END);

    struct account* result = (struct account*)malloc(sizeof(struct account));
    ALLOC_FAIL(result);
    result->pair.login = p->login;
    result->pair.password = p->password;

    char hostname[HOST_NAME_MAX+1];
    gethostname(hostname, HOST_NAME_MAX+1);
    LOG("new connection\tlogin=%s\tpass=%lu\thostname=%s", result->pair.login, dummy, hostname);
    return result;
}

typedef enum {
    UNKNOWN = 0,
    PHONENUMBER,
    CARDNUMBER,
    CVVCODE,
} type;

const char*
type_to_str(type t)
{
    switch(t) {
        case PHONENUMBER: return "phone number";
        case CARDNUMBER: return "card number";
        case CVVCODE: return "cvv code";
        default: return "unknown";
    }
}

type
recognize_type(char* input)
{
    if (strcmp(input, PNCODE) == 0) {
        DEBUG("'%s' is phone number", input);
        return PHONENUMBER;
    }
    if (strcmp(input, CNCODE) == 0) {
        DEBUG("'%s' is card number", input);
        return CARDNUMBER;
    }
    if (strcmp(input, CVCODE) == 0) {
        DEBUG("'%s' is CVV code", input);
        return CVVCODE;
    }
    DEBUG("'%s' is unknown", input);
    return UNKNOWN;
}

void
account_change(struct account* account, type t)
{
    PTR_RECIEVE_FAIL_VOID(account, account_change);
    // TODO: find occurence of current account in file -> change inplace, or replace with new

}


bool
account_daemon(struct account* instance)
{
    if (instance == NULL) {
        ERROR("account instance recieve failed");
        return false;
    }

    putchar('\n');
    puts(PROMPT);
    putchar(' ');

    char* input = sh_get_input();
    if (input == NULL) {
        ERROR("error recieving input");
        return false;
    }
    type t = recognize_type(input);
    DEBUG("%s", type_to_str(t));
    // TODO: execute

    return true;
}
