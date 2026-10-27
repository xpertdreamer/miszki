#include "util.h"
#include "shell.h"

#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define EXECUTABLE_NAME "create"

#define REQUEST_LOGIN "Enter your login: "
#define REQUEST_PASSW "Enter you password: "
#define LOG_SUBDIR    "logs"

int
main(void)
{
    char* login = NULL;
    char* pass = NULL;
    // so, kernel should automatically cleanup pointers here
    // because of it we do not need to handle SIGINT manually


    // paths section
    char current_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", current_path, sizeof(current_path) - 1);
    if (len == -1 || len >= (ssize_t)sizeof(current_path)) {
        ERROR("cannot resolve executable path");
        return EXIT_FAILURE;
    }
    size_t execlen = strlen(EXECUTABLE_NAME);
    current_path[len - execlen] = '\0';
    DEBUG("current executable path=%s", current_path);

    bool log_exists = verify_path(current_path, LOG_SUBDIR, DT_DIR);
    bool success;
    if ((log_exists == false) && (success = create_subdir(current_path, LOG_SUBDIR)) == false) {
        ERROR("cannot create log subdirectory");
    }

    // create logic section
    write(STDOUT_FILENO, REQUEST_LOGIN, strlen(REQUEST_LOGIN));
    login = sh_get_input();
    if (!login || login[0] == '\n') {
        ERROR("no login");
        goto fail;
    }
    DEBUG("login=%s", login);
    write(STDOUT_FILENO, REQUEST_PASSW, strlen(REQUEST_PASSW));
    pass  = sh_get_input();
    if (!pass || pass[0] == '\n') {
        ERROR("no password");
        goto fail;
    }
    DEBUG("password=%s", pass);

    // NOTE: can be replace with goto
    FREE(login, pass);
    return EXIT_SUCCESS;

fail:
    FREE(login, pass);
    return EXIT_FAILURE;
}
