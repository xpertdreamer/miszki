#include "shell.h"

#include <stdlib.h>

int
main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    shell* sh = sh_create(NULL, 0);
    while (sh_daemon(sh));
    sh_destroy(sh);
    return EXIT_SUCCESS;
}
