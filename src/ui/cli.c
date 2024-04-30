#include <pm/ui/cli.h>
#include <pm/ui/cli/proc.h>
#include <pm/audio.h>
#include <pm/sys.h>

#define INPUT_SIZE 512

static FunctionProcessor func_processor[] = 
{
    [FN_NONE]            = process_none,
    [FN_UNKNOWN]         = process_unknown,
    [FN_QUIT]            = process_quit,
    [FN_HELP]            = process_help,
    [FN_START]           = process_start,
    [FN_NEXT]            = process_next_music,
    [FN_PREVIOUS]        = process_previous_music,
    [FN_PAUSE]           = process_pause,
    [FN_PLAY]            = process_play,
    [FN_VOLUME]          = process_volume,
    [FN_SET_TIME]        = process_set_time,
    [FN_LOAD_MUSIC]      = process_load_music,
    [FN_LOAD_MUSIC_DIR]  = process_load_music_directory,
    [FN_UNLOAD_MUSIC]    = process_unload_music,
    [FN_PLAYLIST]        = process_playlist,
    [FN_MUSIC]           = process_music,
    [FN_RENAME_MUSIC]    = process_rename_music,
    [FN_RENAME_PLAYLIST] = process_rename_playlist,
    [FN_SAVE_PLAYLIST]   = process_save_playlist,
};

char *func_name[] = {
    [FN_NONE]            = "FN_NONE",
    [FN_UNKNOWN]         = "FN_UNKNOWN",
    [FN_QUIT]            = "quit",
    [FN_HELP]            = "help",
    [FN_START]           = "start",
    [FN_NEXT]            = "next",
    [FN_PREVIOUS]        = "prev",
    [FN_PAUSE]           = "pause",
    [FN_PLAY]            = "play",
    [FN_VOLUME]          = "vol",
    [FN_SET_TIME]        = "set",
    [FN_LOAD_MUSIC]      = "load",
    [FN_LOAD_MUSIC_DIR]  = "ldir",
    [FN_UNLOAD_MUSIC]    = "rem",
    [FN_PLAYLIST]        = "list",
    [FN_MUSIC]           = "music",
    [FN_RENAME_MUSIC]    = "ren",
    [FN_RENAME_PLAYLIST] = "renlist",
    [FN_SAVE_PLAYLIST]   = "save",
};

char *func_desc[] = {
    [FN_NONE]            = "No function, ignore",
    [FN_UNKNOWN]         = "Unknown function",
    [FN_QUIT]            = "Quit the application",
    [FN_HELP]            = "Print the help message",
    [FN_START]           = "Start the audio playlist",
    [FN_NEXT]            = "Play the next music",
    [FN_PREVIOUS]        = "Play the previous music",
    [FN_PAUSE]           = "Pause the audio",
    [FN_PLAY]            = "Start or resume the audio",
    [FN_VOLUME]          = "Set the audio volume",
    [FN_SET_TIME]        = "Set the music position in second",
    [FN_LOAD_MUSIC]      = "Load a single music",
    [FN_LOAD_MUSIC_DIR]  = "Load a directory of music",
    [FN_UNLOAD_MUSIC]    = "Unload a music in the current playlist",
    [FN_PLAYLIST]        = "Print informations about the playlist",
    [FN_MUSIC]           = "Print informations about the music",
    [FN_RENAME_MUSIC]    = "Rename a music in the current playlist",
    [FN_RENAME_PLAYLIST] = "Rename the current playlist",
    [FN_SAVE_PLAYLIST]   = "Save the current playlist"
};

void cli_start(AppState *state)
{
    /* Initialzation */
    signal(SIGINT, SIG_IGN);
    srand(time(NULL));
    audio_player_initialize();
    
    Playlist *playlist = playlist_create("DEFAULT");
    state->working_playlist = playlist;
    state->playlists = linked_list_create(playlist);
    audio_player_attach_playlist(playlist);
}

void cli_run(AppState *state)
{
    char input[INPUT_SIZE] = {0};
    
    while (state->running)
    {
        printf("> ");
        scanf(" %[^\n]", input);

        Command command;
        command_create(&command, input);

        func_processor[command.fn](state, command);
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
