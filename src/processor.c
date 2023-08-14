#include <playmusic.h>

#include <stdio.h>
#include <stdbool.h>


#define ARGOF(input,fn) (input + strlen(func_name[fn]) + 1)

#define CHECK_PLAYLIST();                           \
if (music_count == 0){                              \
	printf("There's no music in the playlist\n");   \
	return;                                       \
}


extern bool running;
extern bool playing;
extern char *func_name[];
extern char *func_desc[];
extern size_t current_music;
extern size_t music_count;
extern Music *playlist;

void process_none(char *request);

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
	printfunc(FN_HELP,           "fn", "");
	printfunc(FN_RESUME,         "",   "");
	printfunc(FN_PAUSE,          "",   "");
	printfunc(FN_VOLUME,         "vo", "");
	printfarg("vo",              "is a value between 0 and 100");
	printfunc(FN_SET_TIME,       "ti", "");
	printfarg("ti",              "is the time in seconds");
	printfunc(FN_START,          "",   "");
	printfunc(FN_START_RAND,     "",   "");
	printfunc(FN_LOAD_MUSIC,     "pa", "");
	printfarg("pa",              "is the path to the file or directory");
	printfunc(FN_LOAD_MUSIC_DIR, "pa", "ex");
	printfarg("pa",              "is the path to the file or directory");
	printfarg("ex",              "is the file extension");
	printfunc(FN_LIST,           "",   "");
	printfunc(FN_INFO,           "",   "");
}

void process_resume(char *request)
{
	(void) request;
	CHECK_PLAYLIST();
	resume_music();
}

void process_pause(char *request)
{
	(void) request;
	CHECK_PLAYLIST();
	pause_music();
}

void process_volume(char *request)
{
	char *arg = ARGOF(request, FN_VOLUME);

	int percent = atoi(arg);
	if (percent < 0) percent = 0;
	else if (percent > 100) percent = 100;
			
	float volume = (float)percent / 100.f;
			
	printf("Volume level is %.2f\n", volume);
	set_volume(volume);
}

void process_set_time(char *request)
{
	char *sPos = ARGOF(request, FN_SET_TIME);
	if (sPos == NULL)
	{
		printf("Usage: %s <time>\n", func_name[FN_SET_TIME]);
	}
	double pos = atof(sPos);
	seek_music(pos);
}

void process_next_music(char *request)
{
	(void) request;
	next_music();
}

void process_previous_music(char *request)
{
	(void) request;
	previous_music();
}

void process_start(char *request)
{
	(void) request;
	CHECK_PLAYLIST();
	current_music = 0;
	play_music(playlist[current_music]);
	playing = true;
}

void process_start_random(char *request)
{
	(void) request;
	CHECK_PLAYLIST();
	current_music = rand_between(0, music_count - 1);
	play_music(playlist[current_music]);
	playing = true;
}

void process_load_music(char *request)
{
	char *filename = ARGOF(request, FN_LOAD_MUSIC);
	insert_music(filename);
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
	insert_playlist_in_dir(dirname, ext);
}

void process_list(char *request)
{
	(void) request;
	if (music_count == 0)
	{
		printf("There's no music in the playlist\n");
		return;
	}
	
	for (size_t i = 0; i < music_count; ++i)
	{
		if (current_music == i)
			putc('*', stdout);
		else
			putc('-', stdout);
		printf(" %s\n", playlist[i].filename);
	}
}

void process_info(char *request)
{
	(void) request;
	if (music_count != 0)
	{
		printf("%s\n", playlist[current_music].filename);
	}
}