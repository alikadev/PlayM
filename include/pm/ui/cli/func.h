#ifndef PM_UI_CLI_FUNC_H
#define PM_UI_CLI_FUNC_H

#include <pm/ui/cli/command.h>
#include <pm/ui/appState.h>

typedef enum {
    FN_NONE = 0,         // No function, ignore
    FN_UNKNOWN,          // Unknown function, report error
    FN_QUIT,             // Quit the application
    FN_HELP,             // Print the help message
    FN_PAUSE,            // Pause the audio
    FN_VOLUME,           // Set the audio volume
    FN_NEXT,             // Play the next music
    FN_PREVIOUS,         // Play the previous music
    FN_SET_TIME,         // Set the music position in second
    FN_PLAY,             // Start the music or resume it
    FN_START,            // Start the first music
    FN_LOAD_MUSIC,       // Load a single music
    FN_LOAD_MUSIC_DIR,   // Load a directory of music
    FN_UNLOAD_MUSIC,     // Unload a music from the current playlist
    FN_PLAYLIST,         // Print informations about the playlist
    FN_MUSIC,            // Print informations about the music
    FN_RENAME_MUSIC,     // Rename a music
    FN_RENAME_PLAYLIST,  // Rename the current playlist
    FN_SAVE_PLAYLIST,    // Rename the current playlist
    FN_LIST_PLAYLISTS,   // Print all playlists
    FN_CREATE_PLAYLIST,  // Create a playlist
    FN_DESTROY_PLAYLIST, // Destroy a playlist
    FN_SWITCH_PLAYLIST,  // Switch the current playing playlist
    FN_USE_PLAYLIST,     // Switch the current working playlist
} FunctionType;

typedef struct
{
    FunctionType type;
    char name[32];
    char args[16];
    char descr[128];
    void (*processor)(AppState *state, Command cmd);
} Function;

Function *str_to_function(char *name, Function *fns, size_t size);

#endif // PM_UI_CLI_FUNC_H
