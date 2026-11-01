#include "util.h"
#include "log.h"
#include "shell.h"

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define REQUEST_LOGIN "Enter your login: "
#define REQUEST_PASSW "Enter you password: "

int
main(void)
{
    char log_path[PATH_MAX];
    FILE* flog = NULL;

    char* login = NULL;
    char* pass = NULL;

    // path section

    char current_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", current_path, sizeof(current_path) - 1);
    if (len == -1 || len >= (ssize_t)sizeof(current_path)) {
        ERROR("cannot resolve executable path");
        return EXIT_FAILURE;
    }
    size_t execlen = strlen(EXECUTABLE_NAME);
    current_path[len - execlen - 1] = '\0';
    DEBUG("current executable path=%s", current_path);

    bool log_exists;
    bool success;
    bool accounts_exists = verify_path(current_path, DB_SUBDIR, DT_DIR);
    if (accounts_exists == false) {
        ERROR("cannot find db subdirectory");
        goto fail;
    }

    log_exists = verify_path(current_path, LOG_SUBDIR, DT_DIR);
    if ((log_exists == false) && (success = create_subdir(current_path, LOG_SUBDIR)) == false) {
        ERROR("cannot create log subdirectory");
        goto fail;
    }
    if (get_log_file(current_path, LOG_SUBDIR, log_path) == false ) {
        goto fail;
    }
    flog = fopen(log_path, "w");
    if (flog != NULL) {
        log_add_fp(flog, LOG_INFO);
        log_set_quiet(true);
    } else {
        ERROR("cannot open log file");
        goto fail;
    }

    log_info("Account procedure initialized");

    // daemon section

    write(STDOUT_FILENO, REQUEST_LOGIN, strlen(REQUEST_LOGIN));
    login = sh_get_input();
    if (!login) {
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



    log_info("Account procedure ended");
    if (flog != NULL) fclose(flog);
    return EXIT_SUCCESS;

fail:
    if (flog != NULL) {
        log_error("Account procedure failed!");
        fclose(flog);
    }
    return EXIT_FAILURE;
}
