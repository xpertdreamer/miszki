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

    char current_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", current_path, sizeof(current_path) - 1);
    if (len != -1) {
        size_t execlen = strlen(EXECUTABLE_NAME);
        current_path[len - execlen] = '\0';
        DEBUG("current executable path=%s", current_path);
    }
    bool log_exists = verify_path(current_path, LOG_SUBDIR, DT_DIR);
    bool success;
    if ((log_exists == false) && (success = create_subdir(current_path, LOG_SUBDIR)) == false) {
        ERROR("cannot create log subdirectory");
    }

    // NOTE: can be replace with goto
    FREE(login, pass);
    return EXIT_SUCCESS;
}
