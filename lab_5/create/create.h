#ifndef CREATE_H
#define CREATE_H

#include <stdbool.h>

// TODO: function to save data in binary

struct pair {
    char* login;
    char* password;
};

struct pair*
pair_alloc(char* login, char* pass);

void
pair_dealloc(struct pair* p);

void
pair_encrypt();

bool
pair_write();

#endif
