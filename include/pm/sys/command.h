#ifndef PM_SYS_COMMAND_H
#define PM_SYS_COMMAND_H

#include <pm/sys/list.h>

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
    FN_SAVE_PLAYLIST,  // Rename the current playlist
} Function;


typedef struct {
    Function fn;
    LinkedList *tokens;
} Command;

void  command_create(Command *command, char *request);
void  command_destroy(Command *command);

Function str_to_function(char *functionName);



typedef void (*FunctionProcessor)(Command command);

void process_none(Command command);
void process_unknown(Command command);
void process_quit(Command command);
void process_help(Command command);
void process_play(Command command);
void process_pause(Command command);
void process_volume(Command command);
void process_set_time(Command command);
void process_next_music(Command command);
void process_previous_music(Command command);
void process_start(Command command);
void process_load_music(Command command);
void process_load_music_directory(Command command);
void process_unload_music(Command command);
void process_playlist(Command command);
void process_music(Command command);
void process_rename_music(Command command);
void process_rename_playlist(Command command);
void process_save_playlist(Command command);

#endif