#ifndef SHELL_H
#define SHELL_H

#include <signal.h>
#include <stddef.h>
#include <stdbool.h>

// Usage:
// cmd cmds[1] = {{"help", "description", help_handler}}; // initialize vector of commands
// shell *sh = sh_create(cmds, 1);                        // create new shell
// sh_change_prompt(sh, '>');                             // change standard propmt if neededx
// while(sh_daemon(sh));                                  // run daemon to listen user input and execute
// sh_destroy(sh);                                        // free memory on exit


typedef struct Shell shell;

typedef struct {
    const char* name;
    void (*handler)();
} cmd;

// TODO: rewrite commands with hashmap
struct Shell {
    char prompt;
    size_t commands_num;
    cmd* commands;
};

shell*
sh_create(cmd* commands, size_t len);

void
sh_destroy(shell* instance);

void
sh_change_prompt(shell* instance, char newpr);

void
sh_execute(shell* instance, const char* name);

bool
sh_daemon(shell* instance);

char*
sh_get_input();

#endif
