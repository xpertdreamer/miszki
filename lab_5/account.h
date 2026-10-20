#ifndef ACCOUNT_H
#define ACCOUNT_H

struct pair {
    char* login;
    char* password;
    // TODO: other information
};

struct pair*
pair_create(char* login, char* pass);

#endif
