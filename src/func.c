#include <playmusic.h>


#define streq(a,b) (strcmp(a,b) == 0)
extern char *func_name[];

Function str_to_function(char *functionName)
{
	if (strlen(functionName) == 0)
		return FN_NONE;
	if (streq(functionName, func_name[FN_QUIT]))
		return FN_QUIT;
	if (streq(functionName, func_name[FN_HELP]))
		return FN_HELP;
	if (streq(functionName, func_name[FN_RESUME]))
		return FN_RESUME;
	if (streq(functionName, func_name[FN_PAUSE]))
		return FN_PAUSE;
	if (streq(functionName, func_name[FN_VOLUME]))
		return FN_VOLUME;
	if (streq(functionName, func_name[FN_SET_TIME]))
		return FN_SET_TIME;
	if (streq(functionName, func_name[FN_START]))
		return FN_START;
	if (streq(functionName, func_name[FN_START_RAND]))
		return FN_START_RAND;
	if (streq(functionName, func_name[FN_LOAD_MUSIC]))
		return FN_LOAD_MUSIC;
	if (streq(functionName, func_name[FN_LOAD_MUSIC_DIR]))
		return FN_LOAD_MUSIC_DIR;
	if (streq(functionName, func_name[FN_LIST]))
		return FN_LIST;
	if (streq(functionName, func_name[FN_INFO]))
		return FN_INFO;
	if (streq(functionName, func_name[FN_NEXT]))
		return FN_NEXT;
	if (streq(functionName, func_name[FN_PREVIOUS]))
		return FN_PREVIOUS;
	return FN_UNKNOWN;
}