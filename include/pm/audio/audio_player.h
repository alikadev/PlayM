#ifndef PM_AUDIO_DEVICE_H
#define PM_AUDIO_DEVICE_H

#include <pm/audio/playlist.h>

#include <stdbool.h>

// AudioPlayer should not be NULL
void   audio_player_initialize(void);
void   audio_player_terminate(void);
void   audio_player_attach_playlist(Playlist *playlist);
void   audio_player_detach_playlist(void);
void   audio_player_play_first(void);
void   audio_player_play(void);
void   audio_player_play_next(void);
void   audio_player_play_prev(void);
void   audio_player_halt_music(void);
void   audio_player_pause(void);
void   audio_player_seek(double seconds);
void   audio_player_set_volume(int volume);
bool   audio_player_is_ready(void);
bool   audio_player_is_playlist_attached(void);
bool   audio_player_is_playing(void);
bool   audio_player_is_paused(void);
size_t audio_player_current_music_id(void);

#endif
