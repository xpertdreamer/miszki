#include "create.h"
#include "util.h"
#include "shell.h"

#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "log.h"

#define REQUEST_LOGIN "Enter your login: "
#define REQUEST_PASSW "Enter you password: "

int
main(void)
{
    bool success;

    char* login = NULL;
    char* pass = NULL;
    struct pair* p = NULL;

    char log_path[PATH_MAX];
    FILE* flog = NULL;

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
    current_path[len - execlen - 1] = '\0';
    DEBUG("current executable path=%s", current_path);

    // file opening section

    bool log_exists;
    bool accounts_exists = verify_path(current_path, DB_SUBDIR, DT_DIR);
    if ((accounts_exists == false) && (success = create_subdir(current_path, DB_SUBDIR)) == false) {
        ERROR("cannot create db subdirectory");
        goto fail;
    }

    log_exists = verify_path(current_path, LOG_SUBDIR, DT_DIR);
    if ((log_exists == false) && (success = create_subdir(current_path, LOG_SUBDIR)) == false) {
        ERROR("cannot create log subdirectory");
        goto fail;
    }

    if (get_log_file(current_path, LOG_SUBDIR, log_path) == false) {
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

    log_info("Create procedure initialized");

    // create logic section
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

    p = pair_alloc(login, pass);
    if (p == NULL) {
        ERROR("error allocating pair");
        goto fail;
    }

    if (!pair_encrypt(p)) {
        ERROR("failed to encrypt password");
        goto fail;
    }
    DEBUG("hashed=%s : %s", p->login, p->password);

    if (pair_write(p, current_path) == false) {
        goto fail;
    }

    log_info("Create procedure ended successfuly. New user: l=%s, p=%s", p->login, p->password);
    pair_dealloc(p);
    if (flog != NULL) fclose(flog);
    return EXIT_SUCCESS;

fail:
    pair_dealloc(p);
    if (flog != NULL) {
        log_error("Create procedure failed!");
        fclose(flog);
    }
    return EXIT_FAILURE;
}
