#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <assert.h>

#include <pm/ui/cli.h>
#include <pm/sys.h>
#include <pm/audio.h>

bool parse_args(int argc, const char *argv[])
{
    assert(argc > 0 && "Must have at least 1 argument");
    assert(argv && "argv must not be NULL");
    assert(*argv && "Must have at least 1 argument in ARGV");

    const char *program = *argv;
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("Usage: %s (-v / --version)\n", program);
            printf("Flags\n");
            printf("  -v / --version: Print current version\n");
            exit(0);
        }
        else if (strcmp(argv[i], "-v") == 0 
                || strcmp(argv[i], "--version") == 0)
        {
            printf("PlayM Version %d.%d.%d\n",
                    VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
            exit(0);
        }
        else
        {
            fprintf(stderr, "Unknown argument `%s`\n", argv[i]);
            return false;
        }
    }

    return true;
}

int main(int argc, const char *argv[])
{
    if (!parse_args(argc, argv))
        return 1;

    AppState state = {
        .running = true,
        .playing = false,
        .volume = MIX_MAX_VOLUME,
        .playlists = NULL,
    };

    cli_start(&state);
    cli_run(&state);
    cli_stop(&state);
    return 0;
}
