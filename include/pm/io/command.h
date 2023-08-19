#ifndef PM_IO_COMMAND_H
#define PM_IO_COMMAND_H

#include <pm/list.h>

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
    FN_PLAYLIST,         // Print informations about the playlist
    FN_MUSIC,            // Print informations about the music
} Function;


typedef struct {
    Function fn;
    LinkedList *tokens;
} Command;

void  command_create(Command *command, char *request);
void  command_destroy(Command *command);

Function str_to_function(char *functionName);

#endif