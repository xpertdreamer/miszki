#include "util.h"
#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define REQUEST_LOGIN "Enter your login: "
#define REQUEST_PASSW "Enter you password: "

int
main(int argc, char** argv)
{
    DEBUG("create");
    (void)argc;
    (void)argv;

    write(STDOUT_FILENO, REQUEST_LOGIN, strlen(REQUEST_LOGIN));
    char* login = sh_get_input();
    DEBUG("login=%s", login);
    write(STDOUT_FILENO, REQUEST_PASSW, strlen(REQUEST_PASSW));
    char* pass  = sh_get_input();
    DEBUG("password=%s", pass);

    FREE(login, pass);
    return EXIT_SUCCESS;
}
