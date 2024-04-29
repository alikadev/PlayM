#ifndef PM_UI_CLI_PROC_H
#define PM_UI_CLI_PROC_H

#include <pm/ui/cli/commands.h>
#include <pm/audio/playlist.h>

#include <stdbool.h>

typedef struct {
    bool running;
    bool playing;
    int volume;
    Playlist *playlist;
} AppState;

typedef void (*FunctionProcessor)(AppState *state, Command command);

void process_none(AppState *state, Command command);
void process_unknown(AppState *state, Command command);
void process_quit(AppState *state, Command command);
void process_help(AppState *state, Command command);
void process_play(AppState *state, Command command);
void process_pause(AppState *state, Command command);
void process_volume(AppState *state, Command command);
void process_set_time(AppState *state, Command command);
void process_next_music(AppState *state, Command command);
void process_previous_music(AppState *state, Command command);
void process_start(AppState *state, Command command);
void process_load_music(AppState *state, Command command);
void process_load_music_directory(AppState *state, Command command);
void process_unload_music(AppState *state, Command command);
void process_playlist(AppState *state, Command command);
void process_music(AppState *state, Command command);
void process_rename_music(AppState *state, Command command);
void process_rename_playlist(AppState *state, Command command);
void process_save_playlist(AppState *state, Command command);

#endif // PM_UI_CLI_PROC_H
