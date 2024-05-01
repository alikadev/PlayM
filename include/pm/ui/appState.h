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
    size_t working_id;
} AppState;

#endif // PM_UI_CLI_APPSTATE_H
