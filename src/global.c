#include <playmusic.h>

#include <stdbool.h>

volatile bool running = true;
volatile bool playing = false;
int volume = MIX_MAX_VOLUME;

Playlist *playlist = NULL;

FunctionProcessor func_processor[] = 
{
	[FN_NONE]             process_none,
	[FN_UNKNOWN]          process_unknown,
	[FN_QUIT]             process_quit,
	[FN_HELP]             process_help,
	[FN_START]            process_start,
	[FN_NEXT]             process_next_music,
	[FN_PREVIOUS]         process_previous_music,
	[FN_PAUSE]            process_pause,
	[FN_PLAY]             process_play,
	[FN_VOLUME]           process_volume,
	[FN_SET_TIME]         process_set_time,
	[FN_LOAD_MUSIC]       process_load_music,
	[FN_LOAD_MUSIC_DIR]   process_load_music_directory,
	[FN_PLAYLIST]         process_playlist,
	[FN_MUSIC]            process_music,
};

char *func_name[] = {
	[FN_NONE]             "FN_NONE",
	[FN_UNKNOWN]          "FN_UNKNOWN",
	[FN_QUIT]             "quit",
	[FN_HELP]             "help",
	[FN_START]            "start",
	[FN_NEXT]             "next",
	[FN_PREVIOUS]         "prev",
	[FN_PAUSE]            "pause",
	[FN_PLAY]             "play",
	[FN_VOLUME]           "vol",
	[FN_SET_TIME]         "set",
	[FN_LOAD_MUSIC]       "load",
	[FN_LOAD_MUSIC_DIR]   "ldir",
	[FN_PLAYLIST]         "playlist",
	[FN_MUSIC]            "music",
};

char *func_desc[] = {
	[FN_NONE]             "No function, ignore",
	[FN_UNKNOWN]          "Unknown function",
	[FN_QUIT]             "Quit the application",
	[FN_HELP]             "Print the help message",
	[FN_START]            "Start the audio playlist",
	[FN_NEXT]             "Play the next music",
	[FN_PREVIOUS]         "Play the previous music",
	[FN_PAUSE]            "Pause the audio",
	[FN_PLAY]             "Start or resume the audio",
	[FN_VOLUME]           "Set the audio volume",
	[FN_SET_TIME]         "Set the music position in second",
	[FN_LOAD_MUSIC]       "Load a single music",
	[FN_LOAD_MUSIC_DIR]   "Load a directory of music",
	[FN_PLAYLIST]         "Print informations about the playlist",
	[FN_MUSIC]            "Print informations about the music",
};