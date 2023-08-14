#include <playmusic.h>

#include <stdbool.h>

bool running = true;
bool playing = false;
int volume = MIX_MAX_VOLUME;

size_t current_music;
size_t music_count = 0;
Music *playlist = NULL;

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