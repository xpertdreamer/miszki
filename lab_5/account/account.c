#include "util.h"
#include "log.h"
#include "shell.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <dlfcn.h>

#define REQUEST_LOGIN "Enter your login: "
#define REQUEST_PASSW "Enter you password: "

extern void help_handler(void);
extern void exit_handler(void);
extern void change_phone_handler(void);
extern void change_card_handler(void);
extern void change_cvv_handler(void);
extern void show_handler(void);

volatile sig_atomic_t run = 1;

void
handle_sigint(int sig)
{
    (void)sig;
    run = 0;
}

FILE* flog = NULL;
FILE* fbin = NULL;
char* login = NULL;

int
main(void)
{
    signal(SIGINT, handle_sigint);

    char log_path[PATH_MAX];

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

    void* handle = NULL;
    handler enc = get_handler(&handle);
    if (enc == NULL) {
        ERROR("cannot get handler");
        goto fail;
    }

    char login_buf[SIZE];
    char bin_temp[PATH_MAX];
    enc(login, login_buf);
    if (get_bin_file(current_path, DB_SUBDIR, login_buf, bin_temp) == false) {
        ERROR("cannot get bin file");
        goto fail;
    }
    if (verify_file(bin_temp) == false) {
        ERROR("account doesnt exists");
        goto fail;
    }
    fbin = fopen(bin_temp, "r+b");

    cmd cmds[6] = {{"help", help_handler},
                   {"exit", exit_handler},
                   {"show", show_handler},
                   {"chphone", change_phone_handler},
                   {"chcard", change_card_handler},
                   {"chcvv", change_cvv_handler}};
    shell *sh = sh_create(cmds, 6);
    sh_change_prompt(sh, "$");
    while (sh_daemon(sh) && run);

    log_info("Account procedure ended");
    sh_destroy(sh);
    if (fbin != NULL) fclose(fbin);
    if (flog != NULL) fclose(flog);
    if (handle != NULL) dlclose(handle);
    FREE(pass, login);
    return EXIT_SUCCESS;

fail:
    FREE(pass, login);
    if (handle != NULL) dlclose(handle);
    if (fbin != NULL) fclose(fbin);
    if (flog != NULL) {
        log_error("Account procedure failed!");
        fclose(flog);
    }
    return EXIT_FAILURE;
}
