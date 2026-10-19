#include "util.h"
#include "shell.h"

#include <stdio.h>
#include <stdlib.h>

FILE *flog;

void
help_handler()
{
    fprintf(stdout, "Help\n");
}

int
main(int argc, char** argv)
{
    (void)argc;
    char path[MAX_PATH_LEN];
    get_path(argv, path);
    DEBUG("%s", path);
    if ((flog = fopen(path, "a")) == NULL) {
        ERROR("cannot open log file");
        exit(EXIT_FAILURE);
    }

    cmd cmds[1] = {{"help", "description", help_handler}};
    shell *sh = sh_create(cmds, 1);
    sh_change_prompt(sh, '>');
    while(sh_daemon(sh));
    sh_destroy(sh);
    fclose(flog);
}
