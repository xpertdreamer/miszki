#include "shell.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

shell*
sh_create(cmd* commands, size_t len)
{
    PTR_RECIEVE_FAIL_PTR(commands, sh_create);
    shell* sh = NULL;

    sh = (shell*)malloc(sizeof(shell));
    ALLOC_FAIL(sh);
    sh->commands_num = len;
    sh->prompt = '>';
    sh->commands = (cmd*)malloc(sizeof(cmd) * len);
    if (sh->commands == NULL) {
        free(sh);
        return NULL;
    }
    for (size_t i = 0; i < len; ++i) {
        sh->commands[i] = commands[i];
    }

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
}

void
sh_change_prompt(shell* instance, char newpr)
{
    PTR_RECIEVE_FAIL_VOID(instance, sh_change_prompt);
    instance->prompt = newpr;
}

void
sh_execute(shell* instance, const char* name)
{
    PTR_RECIEVE_FAIL_VOID(instance, execute);
    for (size_t i = 0; i < instance->commands_num; ++i) {
        if (strcmp(instance->commands[i].name, name) == 0) {
            DEBUG("called %s", name);
            instance->commands[i].handler();
            return;
        }
    }
    fprintf(stdout, "cannot find command [%s] in current instance\n", name);
}

char*
get_input()
{
    char c = 64;
    char* str = (char*)malloc(10);
    int i = 0;
    scanf("%c", &c);
    while (c != 10) {
        str[i] = c;
        scanf("%c", &c);
        if (i % 10 == 0) {
            str = (char*)realloc(str, i+10);
        }
        i++;
    }
    str[i] = '\0';
    return str;
}

bool
sh_daemon(shell* instance)
{
    if (instance == NULL) {
        ERROR("instance recieve fail");
        return false;
    }
    printf("%c ", instance->prompt);
    char* input = get_input();
    if (input == NULL) {
        ERROR("error recieving input");
        return false;
    }
    sh_execute(instance, input);
    free(input);
    return true;
}
