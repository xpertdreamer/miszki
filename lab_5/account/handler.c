#include "log.h"
#include "util.h"
#include "shell.h"

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define REQST_PHONE "Enter new phone number (8...): "

#define PH_SIZE 11

extern volatile sig_atomic_t run;

extern FILE* fbin;
extern FILE* flog;

void
help_handler(void)
{
    puts("Help:");
    puts("\thelp - show help msg");
    puts("\texit - leave from this account");
    puts("\tshow - show current information");
    puts("\tchphone - change phone number");
}

void
exit_handler(void) {
    run = 0;
}

void
change_phone_handler(void)
{
    log_info("Change phone called");
    if (fbin == NULL) {
        ERROR("cannot change phone, file is not opened");
        log_error("Change phone failed");
        return;
    }
    write(STDOUT_FILENO, REQST_PHONE, strlen(REQST_PHONE));
    char* ph = sh_get_input();
    if (!ph || strlen(ph) != 11) {
        ERROR("no phone");
        return;
    }
    DEBUG("new phone=%s", ph);

    fseek(fbin, SIZE-1, SEEK_SET);
    fwrite(ph, 1, PH_SIZE, fbin);
    log_info("Phone changed: %s", ph);
    FREE(ph);
}

// TODO: bank card number & cvv changing
