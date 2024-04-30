#include <pm/ui/cli/command.h>
#include <pm/audio.h>
#include <pm/debug.h>

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define ARGOF(input,fn) (input + strlen(func_name[fn]) + 1)

extern char *func_name[];
extern char *func_desc[];

void command_create(Command *command, char *request)
{
    debugfn();
    if(!command)
        assert(0 && "Bad arg: command_create 'command' argument is NULL!");
    if(!request)
        assert(0 && "Bad arg: command_create 'request' argument is NULL!");

    command->fn = FN_NONE;
    command->tokens = NULL;

    char *buffer = malloc(strlen(request) + 1);
    size_t i = 0;
    bool backslash = false;
    bool quote = false;

    // Find all the tokens
    while(*request)
    {
        if ((quote && *request == '"')
         || (!quote && !backslash && isspace(*request) && strlen(buffer) > 0))
        {
            // Terminal str
            buffer[i] = 0;
            // Copy str to dest
            char *tok = malloc(strlen(buffer) + 1);
            strcpy(tok, buffer);
            // Put it in token list
            if (!command->tokens)
                command->tokens = linked_list_create(tok);
            else
                linked_list_insert(command->tokens, tok);
            // Restart counter
            i = 0;
            backslash = false;
        }
        else if(*request == '"')
            quote = true;
        else if(*request == '\\' && !backslash)
            backslash = true;
        else {
            buffer[i++] = *request; // Add char
            backslash = false;
        }

        request++;
    }
    if(i > 0)
    {
        // Terminate string
        buffer[i] = 0;
        // Copy str to dest
        char *tok = malloc(strlen(buffer) + 1);
        strcpy(tok, buffer);
        // Put it in token list
        if (!command->tokens)
            command->tokens = linked_list_create(tok);
        else
            linked_list_insert(command->tokens, tok);
    }

    // Parse the first token
    if (linked_list_size(command->tokens) >= 1)
        command->fn = str_to_function(command->tokens->elem);

    free(buffer);
}

void command_destroy(Command *command)
{
    debugfn();
    if(!command)
        assert(0 && "Bad arg: command_destroy 'command' argument is NULL!");

    // Free tokens
    size_t i;
    size_t size = linked_list_size(command->tokens);
    for(i = 0; i < size; i++)
    {
        free(linked_list_get(command->tokens, i));
    }

    linked_list_destroy(command->tokens);
}
