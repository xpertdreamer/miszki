#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    const char* name;
    const char* description;
    void (*handler)();
} cmd;

// TODO: rewrite commands with hashmap
typedef struct {
    char prompt;
    size_t commands_num;
    cmd* commands;
} shell;

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

#endif
