#include "shell.h"
#include "util.h"

#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

extern void help_handler(void);
extern void exit_handler(void);
extern void create_handler(void);
extern void login_handler(void);

volatile sig_atomic_t run = 1;

char lg_temp[PATH_MAX];
char cr_temp[PATH_MAX];

void
handle_sigint(int sig)
{
    (void)sig;
    run = 0;
}

// TODO
int
main(void)
{
    signal(SIGINT, handle_sigint);

    // check up section
    char current_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", current_path, sizeof(current_path) - 1);
    if (len == -1 || len >= (ssize_t)sizeof(current_path)) {
        ERROR("cannot resolve executable path");
        return EXIT_FAILURE;
    }
    size_t execlen = strlen(EXECUTABLE_NAME);
    current_path[len - execlen - 1] = '\0';
    DEBUG("current executable path=%s", current_path);

    if (get_bin_file(current_path, "", "create", cr_temp) == false) {
        ERROR("cannot verify executable `create` exists");
        goto fail;
    }
    if (verify_file(cr_temp) == false) {
        ERROR("executable `create` doesn`t exists");
        goto fail;
    }

    if (get_bin_file(current_path, "", "account", lg_temp) == false) {
        ERROR("cannot verify executable `account` exists");
        goto fail;
    }
    if (verify_file(lg_temp) == false) {
        ERROR("executable `account` doesn`t exists");
        goto fail;
    }

    // shell section
    cmd cmds[4] = {{"help", help_handler},
                   {"exit", exit_handler},
                   {"create", create_handler},
                   {"login", login_handler}};
    shell* sh = sh_create(cmds, 4);
    if (sh == NULL) {
        ERROR("failed to start shell");
        goto fail;
    }
    sh_change_prompt(sh, ">");
    while (sh_daemon(sh) && run);

    // cleanup section
    sh_destroy(sh);
    return EXIT_SUCCESS;

fail:
    sh_destroy(sh);
    return EXIT_FAILURE;
}
