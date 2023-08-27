#ifndef PM_MUSIC_H
#define PM_MUSIC_H

#include <pm/sys/list.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
    Mix_Music *sample;
    char *filename;
    char *name;
} Music;

Music *             music_load_from_file(char *filename);
OrderedLinkedList * music_load_directory(char *dirname, char *ext);
void                music_unload_directory(OrderedLinkedList *musicList);
void                music_unload(Music *music);
int                 music_compare(Music *m1, Music *m2);

#endif