#include "util.h"
#include "account.h"

#include <stdio.h>
#include <stdlib.h>

FILE *flog;

int
main(int argc, char** argv)
{
    // open log file section
    (void)argc;
    char path[MAX_PATH_LEN];
    get_path(argv, path, "log.log");
    DEBUG("%s", path);
    if ((flog = fopen(path, "a")) == NULL) {
        ERROR("cannot open log file");
        exit(EXIT_FAILURE);
    }

    char* login = login_request();
    if (login == NULL) {
        ERROR("failed to request user login");
        fclose(flog);
    }
    char* pass = password_request();
    if (pass == NULL) {
        ERROR("failed to request user password");
        free(login);
        fclose(flog);
    }

    struct pair* p = pair_create(login, pass);
    if (p == NULL) {
        ERROR("failed to create pair");
        free(login);
        free(pass);
        fclose(flog);
    }
    account_create(p);

    fclose(flog);
    free(login);
    free(pass);
}
