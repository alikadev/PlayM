#include <pm/sys.h>
#include <pm/debug.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define streq(a,b) (strcmp(a,b) == 0)
extern char *func_name[];

Function str_to_function(char *functionName)
{
	debugfn();
	if (strlen(functionName) == 0)
		return FN_NONE;
	if (streq(functionName, func_name[FN_QUIT]))
		return FN_QUIT;
	if (streq(functionName, func_name[FN_HELP]))
		return FN_HELP;
	if (streq(functionName, func_name[FN_PLAY]))
		return FN_PLAY;
	if (streq(functionName, func_name[FN_PAUSE]))
		return FN_PAUSE;
	if (streq(functionName, func_name[FN_VOLUME]))
		return FN_VOLUME;
	if (streq(functionName, func_name[FN_SET_TIME]))
		return FN_SET_TIME;
	if (streq(functionName, func_name[FN_START]))
		return FN_START;
	if (streq(functionName, func_name[FN_LOAD_MUSIC]))
		return FN_LOAD_MUSIC;
	if (streq(functionName, func_name[FN_LOAD_MUSIC_DIR]))
		return FN_LOAD_MUSIC_DIR;
	if (streq(functionName, func_name[FN_UNLOAD_MUSIC]))
		return FN_UNLOAD_MUSIC;
	if (streq(functionName, func_name[FN_PLAYLIST]))
		return FN_PLAYLIST;
	if (streq(functionName, func_name[FN_MUSIC]))
		return FN_MUSIC;
	if (streq(functionName, func_name[FN_NEXT]))
		return FN_NEXT;
	if (streq(functionName, func_name[FN_PREVIOUS]))
		return FN_PREVIOUS;
	if (streq(functionName, func_name[FN_RENAME_MUSIC]))
		return FN_RENAME_MUSIC;
	if (streq(functionName, func_name[FN_RENAME_PLAYLIST]))
		return FN_RENAME_PLAYLIST;
	return FN_UNKNOWN;
}

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