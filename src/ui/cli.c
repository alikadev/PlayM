#include <pm/ui/cli.h>
#include <pm/ui/cli/proc.h>
#include <pm/ui/cli/func.h>
#include <pm/audio.h>
#include <pm/sys.h>
#include <termios.h>
#include <dirent.h>
#include <string.h>
#include <wordexp.h>
#include <errno.h>
#ifdef __linux__
#   include <linux/limits.h>
#else
#   include <sys/syslimits.h>
#endif


#define INPUT_SIZE 512

Function functions[] =
{
    {
        FN_NONE,
        "NONE", {0},
        {0},
        process_none
    },
    {
        FN_UNKNOWN,
        {0}, {0},
        {0},
        process_unknown
    },
    {
        FN_QUIT,
        "quit", {0},
        "Quit the application",
        process_quit
    },
    {
        FN_HELP,
        "help", {0},
        "Print Help message",
        process_help
    },
    {
        FN_START,
        "start", {0},
        "Start a playlist from the start", 
        process_start
    },
    {
        FN_NEXT,
        "next", {0},
        "Play the next music",
        process_next_music
    },
    {
        FN_PREVIOUS,
        "prev", {0},
        "Play the previous music",
        process_previous_music
    },
    {
        FN_PAUSE,
        "pause", {0},
        "Pause the music",
        process_pause
    },
    {
        FN_PLAY,
        "play", {0},
        "Continue the music after a pause",
        process_play
    },
    {
        FN_VOLUME,
        "vol", "<vol>",
        "Set the volume between 0 and 100",
        process_volume
    },
    {
        FN_SET_TIME,
        "set", "<time>",
        "Set the time",
        process_set_time
    },
    {
        FN_LOAD_MUSIC,
        "lmus", "<path>",
        "Load a music",
        process_load_music
    },
    {
        FN_UNLOAD_MUSIC,
        "umus", "<path>",
        "Unload a music",
        process_unload_music
    },
    {
        FN_LOAD_MUSIC_DIR,
        "ldir", "<path> (<ext>)",
        "Load all musics of directory",
        process_load_music_directory
    },
    {
        FN_PLAYLIST,
        "plist", {0},
        "Show playlist informations",
        process_playlist
    },
    {
        FN_MUSIC,
        "music", {0},
        "Show playing music informations",
        process_music
    },
    {
        FN_RENAME_MUSIC,
        "ren", "<id> <new>",
        "Rename a music",
        process_rename_music
    },
    {
        FN_RENAME_PLAYLIST,
        "renplist", "<new>",
        "Rename the playlist",
        process_rename_playlist,
    },
    {
        FN_SAVE_PLAYLIST,
        "save", "path",
        "Save the playlist",
        process_save_playlist,
    },
    {
        FN_LIST_PLAYLISTS,
        "plists", {0},
        "List all playlists",
        process_list_playlists,
    },
    {
        FN_CREATE_PLAYLIST,
        "create", "<name>",
        "Create a playlist",
        process_create_playlist,
    },
    {
        FN_DESTROY_PLAYLIST,
        "destroy", {0},
        "Destroy a playlist",
        process_destroy_playlist,
    },
    {
        FN_SWITCH_PLAYLIST,
        "switch", "<name>",
        "Switch working playlist",
        process_switch_playlist,
    },
    {
        FN_USE_PLAYLIST,
        "use", {0},
        "Use working playlist",
        process_use_playlist,
    }
};
size_t function_cnt = 0;

void cli_start(AppState *state)
{
    function_cnt = sizeof functions / sizeof *functions;
    /* Initialzation */
    signal(SIGINT, SIG_IGN);
    srand(time(NULL));
    audio_player_initialize();
    
    Playlist *playlist = playlist_create("DEFAULT");
    state->working_id = 0;
    state->playlists = linked_list_create(playlist);
    audio_player_attach_playlist(playlist);
}

static void list_dir(char *path)
{
    wordexp_t result;
    wordexp(path, &result, 0);
    const char *real_path = result.we_wordv[0];
    if (!real_path)
    {
        fprintf(stderr, "Path must not be empty\n");
        return;
    }
    DIR *dir = opendir(real_path);
    if (!dir)
    {
        fprintf(stderr, "Fail to open directory `%s`: %s\n",
                real_path, strerror(errno));
        return;
    }
 
    struct dirent *ent;
    while ((ent = readdir(dir)))
    {
        printf("- %c %s\n",
                ent->d_type == DT_DIR ? 'D' :
                    ent->d_type == DT_REG ? 'F' :
                    'O',
                ent->d_name);
    }
}

static void get_input(char *buffer, size_t max_size)
{
    char path[PATH_MAX];
    struct termios old, new;
    char ch;
    size_t it = 0, pit = 0;
    
    // Init Termios RAW
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ICANON;
    new.c_lflag &= ECHO;
    tcsetattr(0, TCSANOW, &new);

    while (1)
    {
        if (it + 1 == max_size)
            break;
        
        ch = getchar();
        putc(ch, stdout);

        if (ch == '\n')
            break;
        
        if (ch == 127)
        {
            if (pit > 0)
                pit--;
            if (it > 0)
            {
                it--;

                tcsetattr(0, TCSANOW, &old);
                printf("\b \b");
                tcgetattr(0, &old);
                new = old;
                new.c_lflag &= ICANON;
                new.c_lflag &= ECHO;
                tcsetattr(0, TCSANOW, &new);
            }
            continue;
        }
        if (ch == '\t')
        {
            path[pit] = '\0';
            printf("\n");
            list_dir(path);
            printf("> %.*s", (int)it, buffer);
            continue;
        }
        if (ch == ' ')
            pit = 0;
        else
            path[pit++] = ch;
        
        buffer[it++] = ch;
    }

    // Reset Termios to origin
    tcsetattr(0, TCSANOW, &old);

    buffer[it] = '\0';
}

void cli_run(AppState *state)
{
    char input[INPUT_SIZE] = {0};
    
    while (state->running)
    {
        printf("> ");
        get_input(input, INPUT_SIZE - 1);
        printf("INPUT %s\n", input);
        Command command;
        command_create(&command, input);
        Function *fn;
        if (!command.tokens)
            fn = &functions[0];
        else
            fn = str_to_function(
                    linked_list_get(command.tokens, 0),
                    functions,
                    sizeof functions / sizeof *functions);
        if (!fn)
            fn = &functions[1];
        fn->processor(state, command);
        command_destroy(&command);
    }
}

void cli_stop(AppState *state)
{
    audio_player_halt_music(); 
    audio_player_detach_playlist();

    while (state->playlists)
    {
        Playlist *plist = (Playlist*)state->playlists->elem;
        OrderedLinkedList *node = plist->list;
        while(node)
        {
            music_unload((Music*)node->elem);
            node = node->next;
        }
        playlist_destroy(plist);
        linked_list_remove(&state->playlists, 0);
    }

    audio_player_terminate();
}
