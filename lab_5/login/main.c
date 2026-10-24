#include "util.h"
#include "account.h"

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

FILE *flog;
FILE *fbin;

void
handle_sigint(int sig)
{
    (void)sig;
    _exit(EXIT_SUCCESS);
}

int
main(int argc, char** argv)
{
    signal(SIGINT, handle_sigint);

    // open log file section
    (void)argc;
    char path[MAX_PATH_LEN];
    get_path(argv, path, "log.log");
    if ((flog = fopen(path, "a")) == NULL) {
        ERROR("cannot open log file");
        exit(EXIT_FAILURE);
    }

    // binary file section
    char path_b[MAX_PATH_LEN];
    get_path(argv, path_b, "details.bin");
    if ((fbin = fopen(path_b, "a+b")) == NULL) {
        ERROR("cannot open binary file to write account details");
        exit(EXIT_FAILURE);
    }

    // request section
    enum mode m = mode_request();

    char* login = login_request();
    if (login == NULL) {
        ERROR("failed to request user login");
        exit(EXIT_FAILURE);
    }
    char* pass = password_request();
    if (pass == NULL) {
        ERROR("failed to request user password");
        free(login);
        exit(EXIT_FAILURE);
    }

    // pairing section + daemon
    struct pair* p = pair_create(login, pass);
    if (p == NULL) {
        ERROR("failed to create pair");
        free(login);
        free(pass);
        exit(EXIT_FAILURE);
    }

    switch (m) {
        case CREATE: {
            account_create(p);
            break;
        }
        case LOGIN: {
            struct account* current = account_login(p);
            if (current == NULL) {
                free(login);
                free(pass);
                exit(EXIT_FAILURE);
            }
            // TODO: account daemon
            while (account_daemon(current));
            break;
        }
    }

    fclose(flog);
    fclose(fbin);
    free(login);
    free(pass);
    return EXIT_SUCCESS;
}
