#include <pm/sys.h>
#include <pm/audio.h>
#include <pm/debug.h>

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


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

extern FunctionProcessor func_processor[];

void process_none(Command command)
{
	debugfn();
	(void) command;	
}

void process_unknown(Command command)
{
	debugfn();
	(void) command;
	printf("Unknown function: %s\n", (char *)command.tokens->elem);
}

void process_quit(Command command)
{
	(void) command;
	running = false;
}

void process_help(Command command)
{
// Print function
#define printfunc(fn, a1, a2) \
printf("  %-8s"               \
       " %2s  %2s  "          \
       ": %s\n",              \
       func_name[fn],         \
       a1, a2,                \
       func_desc[fn])

// Print function argument
#define printfarg(arg, desc)  \
printf("  %-8s"               \
       " %2s  %2s  "          \
       "  `%s` %s\n",         \
       "",                    \
       "", "",                \
       arg, desc)

	debugfn();
	(void) command;
	printf("Command list:\n");
	printfunc(FN_QUIT,            "",   "");
	printfunc(FN_HELP,            "",   "");
	printfunc(FN_START,           "",   "");
	printfunc(FN_NEXT,            "",   "");
	printfunc(FN_PREVIOUS,        "",   "");
	printfunc(FN_PAUSE,           "",   "");
	printfunc(FN_PLAY,            "",   "");
	printfunc(FN_VOLUME,          "vo", "");
	printfarg("vo",               "is a value between 0 and 100");
	printfunc(FN_SET_TIME,        "ti", "");
	printfarg("ti",               "is the time in seconds");
	printfunc(FN_LOAD_MUSIC,      "pa", "");
	printfarg("pa",               "is the path to the file or directory");
	printfunc(FN_LOAD_MUSIC_DIR,  "pa", "ex");
	printfarg("pa",               "is the path to the file or directory");
	printfarg("ex",               "is the file extension (Optional, any = *)");
	printfunc(FN_UNLOAD_MUSIC,    "id", "");
	printfarg("id",               "is the music `id` from the `playlist` command");
	printfunc(FN_PLAYLIST,        "",   "");
	printfunc(FN_MUSIC,           "",   "");
	printfunc(FN_RENAME_MUSIC,    "id", "na");
	printfarg("id",               "is the ID of the music from the playlist command");
	printfarg("na",               "is the new music name");
	printfunc(FN_RENAME_PLAYLIST, "na", "");
	printfarg("na",               "if the new playlist name");
}

void process_play(Command command)
{
	debugfn();
	(void) command;
	CHECK_PLAYLIST();
	audio_player_play();
}

void process_pause(Command command)
{
	debugfn();
	(void) command;
	CHECK_PLAYLIST();
	audio_player_pause();
}

void process_volume(Command command)
{
	debugfn();
	if(linked_list_size(command.tokens) != 2)
	{
		fprintf(stderr, "Usage: %s <vol>\n", func_name[command.fn]);
		return;
	}
	char *sVol = linked_list_get(command.tokens, 1);

	int volPercent = atoi(sVol);
	if      (volPercent < 0)   volPercent = 0;
	else if (volPercent > 100) volPercent = 100;
			
	float volume = (float)volPercent / 100.f;
			
	printf("Volume level is %.2f\n", volume);
	audio_player_set_volume(volume);
}

void process_set_time(Command command)
{
	debugfn();
	if(linked_list_size(command.tokens) != 2)
	{
		fprintf(stderr, "Usage: %s <time>\n", func_name[command.fn]);
		return;
	}
	char *sPos = linked_list_get(command.tokens, 1);

	double pos = atof(sPos);
	audio_player_seek(pos);
}

void process_next_music(Command command)
{
	debugfn();
	(void) command;
	audio_player_play_next();
}

void process_previous_music(Command command)
{
	debugfn();
	(void) command;
	audio_player_play_prev();
}

void process_start(Command command)
{
	debugfn();
	(void) command;
	CHECK_PLAYLIST();
	audio_player_play_first();
}

void process_load_music(Command command)
{
	debugfn();
	if(linked_list_size(command.tokens) != 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", func_name[command.fn]);
		return;
	}
	char *filename = linked_list_get(command.tokens, 1);
	Music *music = music_load_from_file(filename);
	if(!music)
		return;
	playlist_insert_music(playlist, music);
}

void process_load_music_directory(Command command)
{
	debugfn();
	size_t tokc = linked_list_size(command.tokens);
	if(tokc != 2 && tokc != 3)
	{
		fprintf(stderr, "Usage: %s <dirname> [<ext>]\n", func_name[command.fn]);
		return;
	}

	char *dirname = linked_list_get(command.tokens, 1);
	char *ext;
	if (tokc == 3)
		ext = linked_list_get(command.tokens, 2);
	else
		ext = "*";

	OrderedLinkedList *musics = music_load_directory(dirname, ext);
	if(!musics)
		return;
	playlist_insert_music_list(playlist, musics);
	// Only destroy the list because the musics are now in the playlist
	ordered_linked_list_destroy(musics);
}

void process_unload_music(Command command)
{
	debugfn();
	if(linked_list_size(command.tokens) != 2)
	{
		fprintf(stderr, "Usage: %s <id>\n", func_name[command.fn]);
		return;
	}

	size_t id = atoi(linked_list_get(command.tokens, 1)) - 1;
	debug("UnloadMusic(id=%lu)\n", id);
	
	if (id == audio_player_current_music_id())
		audio_player_play_next();

	Music *music = ordered_linked_list_remove(&playlist->list, id);
	debug("Unloading %s\n", music->filename);
	music_unload(music);
}

void process_playlist(Command command)
{
	debugfn();
	(void) command;

	printf("Playlist %s\n", playlist->name);
	
	for (size_t i = 0; i < playlist_size(playlist); ++i)
	{
		printf("%3lu%c  %s\n", 
			i + 1, 
		    (audio_player_current_music_id() == i) ? '*' : '.',
			playlist_get_by_order(playlist, i)->name);
	}
}

void process_music(Command command)
{
	debugfn();
	(void) command;

	if (playlist_size(playlist) != 0)
	{
		printf("%s\n", playlist_get_by_order(playlist, audio_player_current_music_id())->filename);
	}
}

void process_rename_music(Command command)
{
	debugfn();
	if(linked_list_size(command.tokens) != 3)
	{
		fprintf(stderr, "Usage: %s <id> <name>\n", func_name[command.fn]);
		return;
	}

	// Get args
	int id = atoi(linked_list_get(command.tokens, 1)) - 1;
	char *name = linked_list_get(command.tokens, 2);
	debug("RenameMusic(id=%d, name=%s)\n", id, name);

	// Get music
	Music *music = (Music*)ordered_linked_list_get(playlist->list, id);
	assert(music && "Internal error: Music is NULL");

	// Rename music
	if(music->name)
		free(music->name);
	music->name = malloc(strlen(name) + 1);
	assert(music->name && "Internal error");
	strcpy(music->name, name);
}

void process_rename_playlist(Command command)
{
	debugfn();
	if(linked_list_size(command.tokens) != 2)
	{
		fprintf(stderr, "Usage: %s <name>\n", func_name[command.fn]);
		return;
	}

	// Get args
	char *name = linked_list_get(command.tokens, 1);
	debug("RenamePlaylist(name=%s)\n", name);

	// Rename playlist
	free(playlist->name);
	playlist->name = malloc(strlen(name) + 1);
	assert(playlist->name && "Internal error: malloc returned NULL");
	strcpy(playlist->name, name);
}
