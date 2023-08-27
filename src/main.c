#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>

#include <pm/sys.h>
#include <pm/audio.h>
#include <pm/debug.h>

extern size_t INPUT_SIZE;

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

	debugfn();

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
		Command command; 
		command_create(&command, input);

		// Process the function
		func_processor[command.fn](command);
		command_destroy(&command);
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