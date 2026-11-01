#include "util.h"

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

extern volatile sig_atomic_t run;

extern char cr_temp[PATH_MAX];
extern char lg_temp[PATH_MAX];

void
help_handler(void)
{
    puts("Help:");
    puts("\thelp - show help message");
    puts("\texit - end session");
    puts("\tcreate - create new account");
    puts("\tlogin - login into account and do something");
}

void
exit_handler(void)
{
    run = 0;
}

void
create_handler(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        ERROR("fork error");
        return;
    }
    if (pid == 0) {
        execlp(cr_temp, "", NULL);
        ERROR("execvp failed");
        return;
    } else {
        int st;
        waitpid(pid, &st, 0);
    }
}

void
login_handler(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        ERROR("fork error");
        return;
    }
    if (pid == 0) {
        execlp(lg_temp, "", NULL);
        ERROR("execvp failed");
        return;
    } else {
        int st;
        waitpid(pid, &st, 0);
    }
}
