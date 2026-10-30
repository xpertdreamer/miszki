#ifndef CREATE_H
#define CREATE_H

#include <stdbool.h>

#define SIZE 65

#ifndef LIB
#define LIB ""
#endif

// TODO: function to save data in binary

struct pair {
    char* login;
    char* password;
};

struct pair*
pair_alloc(char* login, char* pass);

void
pair_dealloc(struct pair* p);

bool
pair_encrypt(struct pair* p);

bool
pair_write(struct pair* p);

#endif
