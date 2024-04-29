#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>

#include <pm/ui/cli.h>
#include <pm/sys.h>
#include <pm/audio.h>
#include <pm/debug.h>

void app_init(const char *argv[]);
void app_start(void);
void app_quit(void);

int main(int argc, const char *argv[])
{
	(void) argc;
	(void) argv;

    AppState state = {
        .running = true,
        .playing = false,
        .volume = MIX_MAX_VOLUME,
        .playlist = NULL,
    };

    cli_start(&state);
    cli_run(&state);
    cli_stop(&state);
	return 0;
}
