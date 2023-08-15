#ifndef PM_MUSIC_H
#define PM_MUSIC_H

#include <pm/list.h>

typedef struct {
    Mix_Music *sample;
    char *filename;
} Music;

Music *             music_load_from_file(char *filename);
OrderedLinkedList * music_load_directory(char *dirname, char *ext);
void                music_unload_directory(OrderedLinkedList *musicList);
void                music_unload(Music *music);
int                 music_compare(Music *m1, Music *m2);

#endif