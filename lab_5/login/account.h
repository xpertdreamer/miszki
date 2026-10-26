#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdbool.h>

enum mode {
    CREATE = 0,
    LOGIN,
};

struct pair {
    char* login;
    char* password;
};

struct account {
    struct pair pair;
    // TODO: other information
};

struct pair*
pair_create(char* login, char* pass);

void
pair_destroy(struct pair* p);

void
account_create(struct pair* p);

struct account*
account_login(struct pair* p);

void
account_destroy(struct account* a);

char*
password_request(void);

char*
login_request(void);

bool
account_daemon(struct account* instance);

#endif
