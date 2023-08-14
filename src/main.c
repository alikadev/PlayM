#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <threads.h>
#include <signal.h>

#include <playmusic.h>

extern bool running;
extern bool playing;
extern int volume;

extern size_t current_music;
extern size_t music_count;
extern Music *playlist;

extern char *func_name[];
extern FunctionProcessor func_processor[];

void app_init(const char *argv[]);
void app_start(void);
void app_quit(void);

int main(int argc, const char *argv[])
{
	(void) argc;

	app_init(argv);

	app_start();

	app_quit();
	return 0;
}

void app_init(const char *argv[])
{
	(void) argv;

	// Ignore CTRL-C
	signal(SIGINT, SIG_IGN);
	
	// Init audio device
	init_audio_device();
	
	// Init rand
	srand(time(NULL));
}

void app_start(void)
{
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
}

void app_quit(void)
{
	stop_music();

	for (size_t i = 0; i < music_count; ++i)
	{
		free_music(playlist[i]);
	}

	if(playlist)
		free(playlist);
	stop_audio_device();
}