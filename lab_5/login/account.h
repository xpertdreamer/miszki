#ifndef ACCOUNT_H
#define ACCOUNT_H

enum mode {
    CREATE = 0,
    LOGIN,
};

struct pair {
    char* login;
    char* password;
    // TODO: other information
};

enum mode
mode_request();

struct pair*
pair_create(char* login, char* pass);

void
account_create(struct pair* p);

char*
password_request(void);

char*
login_request(void);



#endif
