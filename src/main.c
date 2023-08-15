#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <threads.h>
#include <signal.h>

#include <playmusic.h>
#include <pm/debug.h>

extern bool running;
extern bool playing;
extern int volume;

extern Playlist *playlist;

extern char *func_name[];
extern FunctionProcessor func_processor[];

void app_init(const char *argv[]);
void app_start(void);
void app_quit(void);


int main(int argc, const char *argv[])
{
	(void) argc;
	(void) argv;

	/* Initialzation */
	signal(SIGINT, SIG_IGN);
	srand(time(NULL));
	audio_player_initialize();
	playlist = playlist_create("DEFAULT");
	audio_player_attach_playlist(playlist);


	/* Main loop */
	char input[INPUT_SIZE];

	while (running)
	{
		(void) volume;
		// Read the user input
		printf("> ");
		scanf(" %[^\n]", input);
		
		// Transform the input in a Command
		input[strcspn(input, "\n")] = 0;
		char *sfn = strtok(input, " ");
		Function function = str_to_function(sfn);

		// Process the function
		debug("Function [%02X] (%s)\n", function, func_name[function]);
		func_processor[function](input);
	}


	/* Terminate */
	audio_player_halt_music();
	
	OrderedLinkedList *node = playlist->list;
	while(node)
	{
		music_unload((Music*)node->elem);
		node = node->next;
	}

	audio_player_detach_playlist();
	playlist_destroy(playlist);

	audio_player_terminate();
	return 0;
}