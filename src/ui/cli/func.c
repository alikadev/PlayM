#include <stdio.h>
#include <string.h>
#include <pm/debug.h>
#include <pm/ui/cli/func.h>

extern char *func_name[];

Function str_to_function(char *functionName)
{
	debugfn();
	if (strlen(functionName) == 0)
		return FN_NONE;
	if (0 == strcmp(functionName, func_name[FN_QUIT]))
		return FN_QUIT;
	if (0 == strcmp(functionName, func_name[FN_HELP]))
		return FN_HELP;
	if (0 == strcmp(functionName, func_name[FN_PLAY]))
		return FN_PLAY;
	if (0 == strcmp(functionName, func_name[FN_PAUSE]))
		return FN_PAUSE;
	if (0 == strcmp(functionName, func_name[FN_VOLUME]))
		return FN_VOLUME;
	if (0 == strcmp(functionName, func_name[FN_SET_TIME]))
		return FN_SET_TIME;
	if (0 == strcmp(functionName, func_name[FN_START]))
		return FN_START;
	if (0 == strcmp(functionName, func_name[FN_LOAD_MUSIC]))
		return FN_LOAD_MUSIC;
	if (0 == strcmp(functionName, func_name[FN_LOAD_MUSIC_DIR]))
		return FN_LOAD_MUSIC_DIR;
	if (0 == strcmp(functionName, func_name[FN_UNLOAD_MUSIC]))
		return FN_UNLOAD_MUSIC;
	if (0 == strcmp(functionName, func_name[FN_PLAYLIST]))
		return FN_PLAYLIST;
	if (0 == strcmp(functionName, func_name[FN_MUSIC]))
		return FN_MUSIC;
	if (0 == strcmp(functionName, func_name[FN_NEXT]))
		return FN_NEXT;
	if (0 == strcmp(functionName, func_name[FN_PREVIOUS]))
		return FN_PREVIOUS;
	if (0 == strcmp(functionName, func_name[FN_RENAME_MUSIC]))
		return FN_RENAME_MUSIC;
	if (0 == strcmp(functionName, func_name[FN_RENAME_PLAYLIST]))
		return FN_RENAME_PLAYLIST;
	if (0 == strcmp(functionName, func_name[FN_SAVE_PLAYLIST]))
		return FN_SAVE_PLAYLIST;
	return FN_UNKNOWN;
}
