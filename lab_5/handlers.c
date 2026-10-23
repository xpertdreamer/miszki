#include "util.h"

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

extern volatile sig_atomic_t run;

void
help_handler(void)
{
    puts("Help:");
    puts("\thelp\tshow help message");
    puts("\tlogin\tcall account");
}

void
login_handler(void)
{
    char path[MAX_PATH_LEN];
    ssize_t len = readlink("/proc/self/exe", path, MAX_PATH_LEN - 1);
    if (len == -1) {
        ERROR("cant read current directory");
        return;
    }
    path[len] = '\0';
    char *slash = strrchr(path, '/');
    if (slash && strcmp(slash + 1, "lab5") == 0 &&
        (size_t)len + 2 <= sizeof path) {
        strcpy(slash + 1, "login");
    }
    DEBUG("%s", path);
    system(path);
}

void
exit_handler(void)
{
    run = 0;
}
