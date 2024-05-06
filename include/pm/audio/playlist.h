#ifndef PM_AUDIO_PLAYLIST_H
#define PM_AUDIO_PLAYLIST_H

#include <SDL2/SDL_mixer.h>
#include <pm/sys/list.h>
#include <pm/audio/music.h>

typedef struct {
    OrderedLinkedList *list;
    char *name;
} Playlist;

Playlist * playlist_create(const char *name);
void       playlist_destroy(Playlist *playlist);
void       playlist_insert_music(Playlist *playlist, Music *music);
void       playlist_insert_music_list(Playlist *playlist, OrderedLinkedList *musics);
size_t     playlist_size(Playlist *playlist);
Music *    playlist_get_by_order(Playlist *playlist, size_t number);
int        playlist_save_to_m3u(Playlist *playlist, char *filename);
int        playlist_load_m3u(Playlist **p_plist, char *filename);

#endif // PM_AUDIO_PLAYLIST_H
