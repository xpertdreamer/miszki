#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdbool.h>
#include <stdint.h>

typedef uint64_t u64;

#define LOGLEN 4
#define PASSLEN 32
#define PHONELEN 10
#define CARDNUMLEN 16
#define CVVCODELEN 3

#define PNCODE "phone"
#define CNCODE "card_num"
#define CVCODE "cvv"

#define PROMPT "phone/card_num/cvv/show>"
#define PROMPT_LEN 25

#define NOTFOUND -1

struct pair {
    char* login;
    char* password;
};

struct account {
    struct pair pair;
    char phone_number[PASSLEN];
    char card_number[CARDNUMLEN];
    char cvv_code[CVVCODELEN];
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
