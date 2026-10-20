#ifndef ACCOUNT_H
#define ACCOUNT_H

struct pair {
    char* login;
    char* password;
    // TODO: other information
};

struct pair*
pair_create(char* login, char* pass);

void
account_create(struct pair* p);

char*
password_request(void);

char*
login_request(void);

#endif
