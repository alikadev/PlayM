#ifndef PM_UI_CLI_APPSTATE_H
#define PM_UI_CLI_APPSTATE_H

#include <stdbool.h>

#include <pm/audio/playlist.h>
#include <pm/sys/list.h>

typedef struct {
    bool running;
    bool playing;
    int volume;
    LinkedList *playlists;
    Playlist *working_playlist;
} AppState;

#endif // PM_UI_CLI_APPSTATE_H
