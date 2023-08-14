#include <playmusic.h>

#include <stdbool.h>

volatile bool running = true;
volatile bool playing = false;
int volume = MIX_MAX_VOLUME;

size_t current_music;
size_t music_count = 0;
Music *playlist = NULL;

FunctionProcessor func_processor[] = 
{
	[FN_NONE]             process_none,
	[FN_QUIT]             process_quit,
	[FN_HELP]             process_help,
	[FN_UNKNOWN]          process_unknown,
	[FN_RESUME]           process_resume,
	[FN_PAUSE]            process_pause,
	[FN_VOLUME]           process_volume,
	[FN_NEXT]             process_next_music,
	[FN_PREVIOUS]         process_previous_music,
	[FN_SET_TIME]         process_set_time,
	[FN_START]            process_start,
	[FN_START_RAND]       process_start_random,
	[FN_LOAD_MUSIC]       process_load_music,
	[FN_LOAD_MUSIC_DIR]   process_load_music_directory,
	[FN_LIST]             process_list,
	[FN_INFO]             process_info,
};

char *func_name[] = {
	[FN_NONE]             "FN_NONE",
	[FN_QUIT]             "quit",
	[FN_HELP]             "help",
	[FN_UNKNOWN]          "FN_UNKNOWN",
	[FN_RESUME]           "resume",
	[FN_PAUSE]            "pause",
	[FN_VOLUME]           "vol",
	[FN_NEXT]             "next",
	[FN_PREVIOUS]         "prev",
	[FN_SET_TIME]         "set",
	[FN_START]            "play",
	[FN_START_RAND]       "rand",
	[FN_LOAD_MUSIC]       "load",
	[FN_LOAD_MUSIC_DIR]   "ldir",
	[FN_LIST]             "list",
	[FN_INFO]             "info",
};

char *func_desc[] = {
	[FN_NONE]             "No function, ignore",
	[FN_QUIT]             "Quit the application",
	[FN_HELP]             "Print the help message",
	[FN_UNKNOWN]          "Unknown function",
	[FN_RESUME]           "Resume the audio",
	[FN_PAUSE]            "Pause the audio",
	[FN_VOLUME]           "Set the audio volume",
	[FN_NEXT]             "Play the next music",
	[FN_PREVIOUS]         "Play the previous music",
	[FN_SET_TIME]         "Set the music position in second",
	[FN_START]            "Start the audio playlist",
	[FN_START_RAND]       "Start the audio playlist randomly",
	[FN_LOAD_MUSIC]       "Load a single music",
	[FN_LOAD_MUSIC_DIR]   "Load a directory of music",
	[FN_LIST]             "Print the playlist content",
	[FN_INFO]             "Print informations about the music",
};