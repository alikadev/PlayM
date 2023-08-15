#include <playmusic.h>

#include <stdio.h>
#include <stdbool.h>


#define ARGOF(input,fn) (input + strlen(func_name[fn]) + 1)

#define CHECK_PLAYLIST();                           \
if (playlist_size(playlist) == 0){                              \
	printf("There's no music in the playlist\n");   \
	return;                                       \
}


extern bool running;
extern bool playing;
extern char *func_name[];
extern char *func_desc[];
extern Playlist *playlist;

void process_none(char *request);

extern FunctionProcessor func_processor[];

void process_none(char *request)
{
	(void) request;	
}

void process_unknown(char *request)
{
	(void) request;
	printf("Unknown function: %s\n", request);
}

void process_quit(char *request)
{
	(void) request;
	running = false;
}

void process_help(char *request)
{
	(void) request;
	printf("Command list:\n");
	printfunc(FN_QUIT,           "",   "");
	printfunc(FN_HELP,           "",   "");
	printfunc(FN_START,          "",   "");
	printfunc(FN_NEXT,           "",   "");
	printfunc(FN_PREVIOUS,       "",   "");
	printfunc(FN_PAUSE,          "",   "");
	printfunc(FN_PLAY,           "",   "");
	printfunc(FN_VOLUME,         "vo", "");
	printfarg("vo",              "is a value between 0 and 100");
	printfunc(FN_SET_TIME,       "ti", "");
	printfarg("ti",              "is the time in seconds");
	printfunc(FN_LOAD_MUSIC,     "pa", "");
	printfarg("pa",              "is the path to the file or directory");
	printfunc(FN_LOAD_MUSIC_DIR, "pa", "ex");
	printfarg("pa",              "is the path to the file or directory");
	printfarg("ex",              "is the file extension");
	printfunc(FN_PLAYLIST,       "",   "");
	printfunc(FN_MUSIC,          "",   "");
}

void process_play(char *request)
{
	(void) request;
	CHECK_PLAYLIST();
	audio_player_play();
}

void process_pause(char *request)
{
	(void) request;
	CHECK_PLAYLIST();
	audio_player_pause();
}

void process_volume(char *request)
{
	char *arg = ARGOF(request, FN_VOLUME);

	int percent = atoi(arg);
	if (percent < 0) percent = 0;
	else if (percent > 100) percent = 100;
			
	float volume = (float)percent / 100.f;
			
	printf("Volume level is %.2f\n", volume);
	audio_player_set_volume(volume);
}

void process_set_time(char *request)
{
	char *sPos = ARGOF(request, FN_SET_TIME);
	if (sPos == NULL)
	{
		printf("Usage: %s <time>\n", func_name[FN_SET_TIME]);
	}
	double pos = atof(sPos);
	audio_player_seek(pos);
}

void process_next_music(char *request)
{
	(void) request;
	audio_player_play_next();
}

void process_previous_music(char *request)
{
	(void) request;
	audio_player_play_prev();
}

void process_start(char *request)
{
	(void) request;
	CHECK_PLAYLIST();
	audio_player_play_first();
}

void process_load_music(char *request)
{
	char *filename = ARGOF(request, FN_LOAD_MUSIC);
	playlist_insert_music(playlist, music_load_from_file(filename));
}

void process_load_music_directory(char *request)
{
	char *dirname = ARGOF(request, FN_LOAD_MUSIC_DIR);
	if (dirname == NULL)
	{
		printf("Usage: %s <dirname> <ext>\n", func_name[FN_LOAD_MUSIC_DIR]);
		return;
	}
	char *ext = strtok(dirname, " ");
	while (*ext)
		ext++;
	ext++;
	if (ext == NULL)
	{
		printf("Usage: %s <dirname> <ext>\n", func_name[FN_LOAD_MUSIC_DIR]);
		return;
	}
	OrderedLinkedList *musics = music_load_directory(dirname, ext);
	playlist_insert_music_list(playlist, musics);
	// Only destroy the list because the musics are now in the playlist
	ordered_linked_list_destroy(musics);
}

void process_playlist(char *request)
{
	(void) request;

	printf("Playlist %s\n", playlist->name);
	
	for (size_t i = 0; i < playlist_size(playlist); ++i)
	{
		if (audio_player_current_music_id() == i)
			putc('*', stdout);
		else
			putc('-', stdout);
		printf(" %s\n", playlist_get_by_order(playlist, i)->filename);
	}
}

void process_music(char *request)
{
	(void) request;

	if (playlist_size(playlist) != 0)
	{
		printf("%s\n", playlist_get_by_order(playlist, audio_player_current_music_id())->filename);
	}
}