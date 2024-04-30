#ifndef PM_UI_CLI_APPSTATE_H
#define PM_UI_CLI_APPSTATE_H

#include <stdbool.h>

#include <pm/audio/playlist.h>

typedef struct {
    bool running;
    bool playing;
    int volume;
    Playlist *playlist;
} AppState;

#endif // PM_UI_CLI_APPSTATE_H
