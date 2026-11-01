#include <stdio.h>
#include <signal.h>

extern volatile sig_atomic_t run;

void
help_handler(void)
{
    puts("Help:");
    puts("\thelp - show help msg");
    puts("\texit - leave from this account");
}

void
exit_handler(void) {
    run = 0;
}
