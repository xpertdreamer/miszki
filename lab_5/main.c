#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_PATH_LEN 1024

FILE *flog;

int
main(int argc, char** argv)
{
    char buf[MAX_PATH_LEN];
    char* realp = realpath(argv[0], buf);
    if (!realp) {
        ERROR("cannot get real path");
        exit(EXIT_FAILURE);
    }
    buf[strlen(buf) - 4] = '\0';
    (void)argc;
    char path[MAX_PATH_LEN];
    snprintf(path, MAX_PATH_LEN, "%.1000slog.log", buf);
    DEBUG("%s", path);
    if ((flog = fopen(path, "a")) == NULL) {
        ERROR("cannot open log file");
        exit(EXIT_FAILURE);
    }
    LOG("try");
    fclose(flog);
}
