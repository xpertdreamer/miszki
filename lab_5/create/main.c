// NOTE: find can i use same memory area in different executables

#include "util.h"
#include "account.h"

#include <signal.h>

FILE *flog;
FILE *fbin;

volatile sig_atomic_t sig_exit = 0;

void
handle_sigint(int sig)
{
    (void)sig;
    exit(EXIT_SUCCESS);
}

int
main(int argc, char** argv)
{
    signal(SIGINT, handle_sigint);

    // log file section
    (void)argc;
    char path[MAX_PATH_LEN];
    get_path(argv, path, "log.log");
    if ((flog = fopen(path, "a")) == NULL) {
        ERROR("cannot open log file");
        exit(EXIT_FAILURE);
    }

    // binary files section
    char path_b[MAX_PATH_LEN];
    get_path(argv, path_b, "details.bin"); // TODO: rename to pairs
    if ((fbin = fopen(path_b, "a+b")) == NULL) {
        ERROR("cannot open binary file to write account details");
        exit(EXIT_FAILURE);
    }
    /* get_path(argv, path_b, "details.bin"); */
    /* if ((fbin = fopen(path_b, "a+b")) == NULL) { */
    /*     ERROR("cannot open binary file to write account details"); */
    /*     exit(EXIT_FAILURE); */
    /* } */


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

    struct pair* p = pair_create(login, pass);
    if (p == NULL) {
        ERROR("failed to create pair");
        free(login);
        free(pass);
        exit(EXIT_FAILURE);
    }

    account_create(p);

    fclose(flog);
    fclose(fbin);
    free(login);
    free(pass);
    return EXIT_SUCCESS;
}
