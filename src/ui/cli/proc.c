#include <pm/ui/cli/proc.h>
#include <pm/audio.h>
#include <assert.h>
#include <assert.h>

extern char *func_name[];
extern char *func_desc[];

#define CHECK_PLAYLIST(playlist);                   \
if (playlist_size(playlist) == 0){                  \
    printf("There's no music in the playlist\n");   \
    return;                                         \
}

void process_none(AppState *state, Command command)
{
    (void) command;    
    (void) state;
}

void process_unknown(AppState *state, Command command)
{
    (void) command;
    (void) state;
    printf("Unknown function: %s\n", (char *)command.tokens->elem);
}

void process_quit(AppState *state, Command command)
{
    (void) command;
    state->running = false;
}

void process_help(AppState *state, Command command)
{
    (void) command;
    (void) state;
    printf("Command list:\n");
    printf("%s: %s\n", func_name[FN_QUIT], func_desc[FN_QUIT]);
    printf("%s: %s\n", func_name[FN_HELP], func_desc[FN_HELP]);
    printf("%s: %s\n", func_name[FN_START], func_desc[FN_START]);
    printf("%s: %s\n", func_name[FN_NEXT], func_desc[FN_NEXT]);
    printf("%s: %s\n", func_name[FN_PREVIOUS], func_desc[FN_PREVIOUS]);
    printf("%s: %s\n", func_name[FN_PAUSE], func_desc[FN_PAUSE]);
    printf("%s: %s\n", func_name[FN_PLAY], func_desc[FN_PLAY]);
    printf("%s <0-100>: %s\n", 
            func_name[FN_VOLUME], func_desc[FN_VOLUME]);
    printf("%s <time>: %s\n", 
            func_name[FN_SET_TIME], func_desc[FN_SET_TIME]);
    printf("%s <path>: %s\n", 
            func_name[FN_LOAD_MUSIC], func_desc[FN_LOAD_MUSIC]);
    printf("%s <path> (<ext>): %s\n", 
            func_name[FN_LOAD_MUSIC_DIR], func_desc[FN_LOAD_MUSIC_DIR]);
    printf("%s <id>: %s\n", 
            func_name[FN_UNLOAD_MUSIC], func_desc[FN_UNLOAD_MUSIC]);
    printf("%s: %s\n", func_name[FN_PLAYLIST], func_desc[FN_PLAYLIST]);
    printf("%s: %s\n", func_name[FN_MUSIC], func_desc[FN_MUSIC]);
    printf("%s <id> <name>: %s\n", 
            func_name[FN_RENAME_MUSIC], func_desc[FN_RENAME_MUSIC]);
    printf("%s <id> <name>: %s\n", 
            func_name[FN_RENAME_PLAYLIST], func_desc[FN_RENAME_PLAYLIST]);
    printf("%s <name>: %s\n", 
            func_name[FN_SAVE_PLAYLIST], func_desc[FN_SAVE_PLAYLIST]);
    printf("%s: %s\n", 
            func_name[FN_LIST_PLAYLISTS], func_desc[FN_LIST_PLAYLISTS]);
    printf("%s <name>: %s\n", 
            func_name[FN_CREATE_PLAYLIST], func_desc[FN_CREATE_PLAYLIST]);
    printf("%s <name>: %s\n", 
            func_name[FN_DESTROY_PLAYLIST], func_desc[FN_DESTROY_PLAYLIST]);
    printf("%s: %s\n", 
            func_name[FN_SWITCH_PLAYLIST], func_desc[FN_SWITCH_PLAYLIST]);
    printf("%s <name>: %s\n", 
            func_name[FN_USE_PLAYLIST], func_desc[FN_USE_PLAYLIST]);
}

void process_play(AppState *state, Command command)
{
    (void) state;
    (void) command;
    if (!audio_player_is_playlist_attached())
    {
        printf("No playlist is currently attached\n");
        return;
    }
    audio_player_play();
}

void process_pause(AppState *state, Command command)
{
    (void) state;
    (void) command;
    audio_player_pause();
}

void process_volume(AppState *state, Command command)
{
    if(linked_list_size(command.tokens) != 2)
    {
        fprintf(stderr, "Usage: %s <vol>\n", func_name[command.fn]);
        return;
    }
    char *sVol = linked_list_get(command.tokens, 1);

    int volPercent = atoi(sVol);
    if      (volPercent < 0)   volPercent = 0;
    else if (volPercent > 100) volPercent = 100;
    
    state->volume = (float)volPercent / 100.f * MIX_MAX_VOLUME;
            
    printf("Volume level is %d\n", volPercent);
    audio_player_set_volume(state->volume);
}

void process_set_time(AppState *state, Command command)
{
    (void) state;
    if(linked_list_size(command.tokens) != 2)
    {
        fprintf(stderr, "Usage: %s <time>\n", func_name[command.fn]);
        return;
    }
    char *sPos = linked_list_get(command.tokens, 1);

    double pos = atof(sPos);
    audio_player_seek(pos);
}

void process_next_music(AppState *state, Command command)
{
    (void) command;
    (void) state;
    if (!audio_player_is_playlist_attached())
    {
        printf("No playlist is currently attached\n");
        return;
    }
    audio_player_play_next();
}

void process_previous_music(AppState *state, Command command)
{
    (void) command;
    (void) state;
    if (!audio_player_is_playlist_attached())
    {
        printf("No playlist is currently attached\n");
        return;
    }
    audio_player_play_prev();
}

void process_start(AppState *state, Command command)
{
    (void) state;
    (void) command;
    if (!audio_player_is_playlist_attached())
    {
        printf("No playlist is currently attached\n");
        return;
    }
    audio_player_play_first();
}

void process_load_music(AppState *state, Command command)
{
    if(linked_list_size(command.tokens) != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", func_name[command.fn]);
        return;
    }
    char *filename = linked_list_get(command.tokens, 1);
    Music *music = music_load_from_file(filename);
    if(!music)
        return;
    Playlist *plist = linked_list_get(state->playlists, state->working_id);
    playlist_insert_music(plist, music);
}

void process_load_music_directory(AppState *state, Command command)
{
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
    Playlist *plist = linked_list_get(state->playlists, state->working_id);
    playlist_insert_music_list(plist, musics);
    // Only destroy the list because the musics are now in the playlist
    ordered_linked_list_destroy(musics);
}

void process_unload_music(AppState *state, Command command)
{
    if(linked_list_size(command.tokens) != 2)
    {
        fprintf(stderr, "Usage: %s <id>\n", func_name[command.fn]);
        return;
    }

    size_t id = atoi(linked_list_get(command.tokens, 1)) - 1;
    
    if (id == audio_player_current_music_id())
        audio_player_play_next();

    Playlist *plist = linked_list_get(state->playlists, state->working_id);
    Music *music = ordered_linked_list_remove(&plist->list, id);
    music_unload(music);
}

void process_playlist(AppState *state, Command command)
{
    (void) command;

    Playlist *plist = linked_list_get(state->playlists, state->working_id);
    printf("Playlist %s\n", plist->name);
    
    for (size_t i = 0; i < playlist_size(plist); ++i)
    {
        printf("%3lu%c  %s\n", 
            i + 1, 
            (audio_player_current_music_id() == i) ? '*' : '.',
            playlist_get_by_order(plist, i)->name);
    }
}

void process_music(AppState *state, Command command)
{
    (void) state;
    (void) command;

    Playlist *playlist = audio_player_get_attached_playlist();
    if (!playlist)
    {
        printf("No playlist is currently attached\n");
        return;
    }

    if (playlist_size(playlist) == 0)
    {
        printf("Current playlist is empty\n");
        return;
    }

    size_t id = audio_player_current_music_id();

    printf("%s\n", playlist_get_by_order(playlist, id)->filename);
}

void process_rename_music(AppState *state, Command command)
{
    if(linked_list_size(command.tokens) != 3)
    {
        fprintf(stderr, "Usage: %s <id> <name>\n", func_name[command.fn]);
        return;
    }

    // Get args
    int id = atoi(linked_list_get(command.tokens, 1)) - 1;
    char *name = linked_list_get(command.tokens, 2);

    // Get music
    Playlist *plist = linked_list_get(state->playlists, state->working_id);
    Music *music = (Music*)ordered_linked_list_get(
            plist->list, id);
    assert(music && "Internal error: Music is NULL");

    // Rename music
    if(music->name)
        free(music->name);
    music->name = malloc(strlen(name) + 1);
    assert(music->name && "Internal error");
    strcpy(music->name, name);
}

void process_rename_playlist(AppState *state, Command command)
{
    if(linked_list_size(command.tokens) != 2)
    {
        fprintf(stderr, "Usage: %s <name>\n", func_name[command.fn]);
        return;
    }

    // Get args
    char *name = linked_list_get(command.tokens, 1);

    // Rename playlist
    Playlist *plist = linked_list_get(state->playlists, state->working_id);
    free(plist->name);
    plist->name = malloc(strlen(name) + 1);
    assert(plist->name && "Internal error: malloc returned NULL");
    strcpy(plist->name, name);
}


void process_save_playlist(AppState *state, Command command)
{
    if(linked_list_size(command.tokens) != 2)
    {
        fprintf(stderr, "Usage: %s <path>\n", func_name[command.fn]);
        return;
    }

    char *path = linked_list_get(command.tokens, 1);

    Playlist *plist = linked_list_get(state->playlists, state->working_id);
    playlist_save_to_m3u(plist, path);
}

void process_list_playlists(AppState *state, Command command)
{
    (void) command;
    LinkedList *it = state->playlists;
    while(it)
    {
        Playlist *plist = (Playlist*) it->elem;
        printf("- %s\n", plist->name);
        it = it->next;
    }
}

void process_create_playlist(AppState *state, Command command)
{
    if (linked_list_size(command.tokens) != 2)
    {
        fprintf(stderr, "Usage: %s <name>\n", func_name[command.fn]);
        return;
    }

    const char *name = linked_list_get(command.tokens, 1);

    Playlist *new_plist = playlist_create(name);
    linked_list_insert(state->playlists, new_plist);

    state->working_id = linked_list_size(state->playlists) - 1;
}

void process_destroy_playlist(AppState *state, Command command)
{
    (void) command;
    if (state->working_id == 0)
    {
        fprintf(stderr, "Cannot destroy the default playlist\n");
        return;
    }

    // Switch playlist if working playlist is attached playlist
    Playlist *plist = linked_list_get(state->playlists, state->working_id);
    if (audio_player_get_attached_playlist() == plist)
    {
        printf("Destroying playing playlist, playling playlist is now the default one\n");
        audio_player_pause();
        audio_player_attach_playlist(linked_list_get(state->playlists, 0));
    }

    playlist_destroy(plist);
    linked_list_remove(&state->playlists, state->working_id);
    state->working_id = 0;
    printf("Done\n");
}

void process_switch_playlist(AppState *state, Command command)
{
    if (linked_list_size(command.tokens) != 2)
    {
        fprintf(stderr, "Usage: %s <name>\n", func_name[command.fn]);
        return;
    }

    const char *name = linked_list_get(command.tokens, 1);
    for (size_t id = 0; id < linked_list_size(state->playlists); ++id)
    {
        Playlist *plist = linked_list_get(state->playlists, id);
        if (strcmp(plist->name, name) == 0)
        {
            state->working_id = id;
            printf("Now switched to %s\n", name);
            return;
        }
    }

    fprintf(stderr, "Playlist `%s` not founded\n", name);
}

void process_use_playlist(AppState *state, Command command)
{
    (void) command;
    Playlist *plist = linked_list_get(state->playlists, state->working_id);
    if (plist == audio_player_get_attached_playlist())
    {
        fprintf(stderr, "Playlist %s is already attached\n", plist->name);
        return;
    }

    audio_player_pause();
    audio_player_attach_playlist(plist);
    printf("Done\n");
}

