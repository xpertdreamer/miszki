#include "log.h"
#include "util.h"
#include "shell.h"

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define SHOW_LOGIN "Login: "
#define SHOW_PHONE "Phone number: "
#define SHOW_CARDN "Card number: "
#define SHOW_CVVCD "CVV: "

#define REQST_PHONE "Enter new phone number (8...): "
#define REQST_CVVCD "Enter new cvv (3 digits): "
#define REQST_CARDN "Enter new card number (16 digits): "

#define PH_SIZE 11
#define CN_SIZE 16
#define CV_SIZE 3

extern volatile sig_atomic_t run;

extern char* login;
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
show_handler(void)
{
    log_info("Started to show account information");
    if (fbin == NULL || login == NULL) {
        ERROR("cannot show info, file is not opened");
        log_error("Show failed");
        return;
    }
    write(STDOUT_FILENO, SHOW_LOGIN, strlen(SHOW_LOGIN));
    write(STDOUT_FILENO, login, strlen(login));
    write(STDOUT_FILENO, "\n", 1);

    write(STDOUT_FILENO, SHOW_PHONE, strlen(SHOW_PHONE));
    fseek(fbin, SIZE-1, SEEK_SET);
    char ph[PH_SIZE];
    if (fread(ph, 1, PH_SIZE, fbin) != PH_SIZE) {
        ERROR("cannot read phone number");
        return;
    }
    write(STDOUT_FILENO, ph, PH_SIZE);
    write(STDOUT_FILENO, "\n", 1);

    write(STDOUT_FILENO, SHOW_CARDN, strlen(SHOW_CARDN));
    fseek(fbin, SIZE-1+PH_SIZE, SEEK_SET);
    char cn[CN_SIZE];
    if (fread(cn, 1, CN_SIZE, fbin) != CN_SIZE) {
        ERROR("cannot read card number");
        return;
    }
    write(STDOUT_FILENO, cn, CN_SIZE);
    write(STDOUT_FILENO, "\n", 1);

    write(STDOUT_FILENO, SHOW_CVVCD, strlen(SHOW_CVVCD));
    fseek(fbin, SIZE-1+PH_SIZE+CN_SIZE, SEEK_SET);
    char cv[CV_SIZE];
    if (fread(cv, 1, CV_SIZE, fbin) != CV_SIZE) {
        ERROR("cannot read cvv code");
        return;
    }
    write(STDOUT_FILENO, cv, CV_SIZE);
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

    if (fseek(fbin, SIZE-1, SEEK_SET) != 0) {
        ERROR("cannot find number section");
        return;
    }
    fwrite(ph, 1, PH_SIZE, fbin);
    log_info("Phone changed: %s", ph);
    FREE(ph);
}

void
change_card_handler(void)
{
    log_info("Change car number called");
    if (fbin == NULL) {
        ERROR("cannot change card number, file is not opened");
        log_error("Change card number failed");
        return;
    }
    write(STDOUT_FILENO, REQST_CARDN, strlen(REQST_CARDN));
    char* cn = sh_get_input();
    if (!cn || strlen(cn) != CN_SIZE) {
        ERROR("no card number");
        return;
    }
    DEBUG("new card number=%s", cn);

    if (fseek(fbin, SIZE-1+PH_SIZE, SEEK_SET) != 0) {
        ERROR("cannot find card number section|insert phone number");
        return;        
    }
    fwrite(cn, 1, CN_SIZE, fbin);
    log_info("Card number changed: %s", cn);
    FREE(cn);
}

void
change_cvv_handler(void)
{
    log_info("Change cvv called");
    if (fbin == NULL) {
        ERROR("cannot change cvv, file is not opened");
        log_error("Change cvv failed");
        return;
    }
    write(STDOUT_FILENO, REQST_CVVCD, strlen(REQST_CVVCD));
    char* cv = sh_get_input();
    if (!cv || strlen(cv) != CV_SIZE) {
        ERROR("no cvv");
        return;
    }
    DEBUG("new cvv=%s", cv);
    if (fseek(fbin, SIZE-1+PH_SIZE+CN_SIZE, SEEK_SET) != 0) {
        ERROR("cannot find cvv section|insert card number");
        return;
    }
    fwrite(cv, 1, CN_SIZE, fbin);
    log_info("CVV changed: %s", cv);
    FREE(cv);
}

// TODO: bank card number & cvv changing
