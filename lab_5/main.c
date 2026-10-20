#include "util.h"
#include "shell.h"

#include <stdio.h>
#include <stdlib.h>

FILE *flog;

extern void help_handler(void);
extern void login_handler(void);

int
main(int argc, char** argv)
{
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
    cmd cmds[2] = {{"help", help_handler},
                   {"login", login_handler}};
    shell *sh = sh_create(cmds, 2);
    sh_change_prompt(sh, '>');

    while(sh_daemon(sh));

    // cleanup section
    sh_destroy(sh);
    fclose(flog);
}
