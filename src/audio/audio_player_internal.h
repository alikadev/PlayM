#ifndef PM_AUDIO_PLAYER_INTERNAL_H
#define PM_AUDIO_PLAYER_INTERNAL_H

#define AP_STATE_READY     (1 << 0)
#define AP_STATE_NONE      0

typedef struct 
{    
    Playlist *playlist;
    size_t currentMusic;
    unsigned char state;
} AudioPlayer;

#endif