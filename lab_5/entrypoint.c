#include "shell.h"

#include <stdlib.h>

int
main(void)
{
    // shell section
    cmd cmds[4] = {{"help", NULL},
                  {"exit", NULL},
                  {"create", NULL},
                  {"login", NULL}};
    shell* sh = sh_create(cmds, 4);
    sh_change_prompt(sh, "$");
    while (sh_daemon(sh));

    // cleanup section
    sh_destroy(sh);
    return EXIT_SUCCESS;
}
