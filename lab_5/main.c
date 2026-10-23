#include "util.h"
#include "shell.h"

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

FILE *flog;

volatile sig_atomic_t run = 1;

void
handle_sigint(int sig)
{
    (void)sig;
    run = 0;
}

extern void help_handler(void);
extern void account_handler(void);
extern void exit_handler(void);

int
main(int argc, char** argv)
{
    signal(SIGINT, handle_sigint);

    // open log file section
    char path[MAX_PATH_LEN];
    (void)argc;
    get_path(argv, path, "log.log");
    DEBUG("%s", path);
    if ((flog = fopen(path, "a")) == NULL) {
        ERROR("cannot open log file");
        exit(EXIT_FAILURE);
    }

    // shell section
    cmd cmds[3] = {{"help", help_handler},
                   {"account", account_handler},
                   {"exit", exit_handler}};
    shell *sh = sh_create(cmds, 3);
    sh_change_prompt(sh, '$');

    while(run && sh_daemon(sh));

    // cleanup section
    sh_destroy(sh);
    fclose(flog);
}
