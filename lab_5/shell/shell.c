#include "util.h"

#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

shell*
sh_create(cmd* commands, size_t len)
{
    PTR_RECIEVE_FAIL_PTR(commands, sh_create);
    shell *sh = NULL;

    sh = (shell*)malloc(sizeof(shell));
    ALLOC_FAIL(sh);
    sh->num_commands = len;
    sh->prompt = ">";
    sh->commands = (cmd*)malloc(sizeof(cmd) * len);
    if (sh->commands == NULL) {
        free(sh);
        return NULL;
    }
    for (size_t i = 0; i < len; ++i) {
        sh->commands[i] = commands[i];
    }
    // TODO: log creation

    return sh;
}

void
sh_destroy(shell* instance)
{
    if (instance != NULL) {
        if (instance->commands != NULL) {
            free(instance->commands);
        }
        free(instance);
    }
    // TODO: log destroying
}

void
sh_change_prompt(shell* instance, const char* newpr)
{
    PTR_RECIEVE_FAIL_VOID(instance, sh_change_prompt);
    instance->prompt = newpr;
    instance->prompt_len = (u32)strlen(newpr);
}

void
sh_execute(shell* instance, const char* name)
{
    PTR_RECIEVE_FAIL_VOID(instance, execute);
    for (size_t i = 0; i < instance->num_commands; ++i) {
        if (strcmp(instance->commands[i].name, name) == 0) {
            DEBUG("called %s", name);
            instance->commands[i].handler();
            return;
        }
    }
    ERROR("cannot find command [%s] in current instance\n", name);
}

char*
sh_get_input()
{
    return NULL;
}

bool
sh_daemon(shell* instance)
{
    if (instance == NULL) {
        ERROR("instance recieve fail");
        return false;
    }

    putchar('\n');
    write(STDOUT_FILENO, instance->prompt, instance->prompt_len);
    putchar(' ');

    char* input = sh_get_input();
    if (input == NULL) {
        ERROR("error recieving input");
        return false;
    }
    sh_execute(instance, input);

    free(input);
    return true;
}
