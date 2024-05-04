#include <pm/audio/playlist.h>
#include <pm/sys/asserts.h>

#include <string.h>
#include <errno.h> 
#include <stdlib.h>

extern int errno;

Playlist *playlist_create(const char *name)
{
    ARG_ASSERT(name);
    Playlist *playlist = malloc(sizeof *playlist);
    ALLOC_ASSERT(playlist);

    playlist->name = malloc(strlen(name) + 1);
    ALLOC_ASSERT(playlist->name);
    strcpy(playlist->name, name);

    playlist->list = NULL;
    return playlist;
}

void playlist_destroy(Playlist *playlist)
{
    ARG_ASSERT(playlist);

    if(playlist->name)
        free(playlist->name);

    if(playlist->list)
        ordered_linked_list_destroy(playlist->list);

    free(playlist);
}

void playlist_insert_music(Playlist *playlist, Music *music)
{
    ARG_ASSERT(playlist);
    ARG_ASSERT(music);

    if(!playlist->list)
    {
        playlist->list = ordered_linked_list_create(music, (int(*)(void*,void*))music_compare);
    } else {
        ordered_linked_list_insert(&playlist->list, music);
    }
}

void playlist_insert_music_list(Playlist *playlist, OrderedLinkedList *musics)
{
    ARG_ASSERT(playlist);
    ARG_ASSERT(musics);

    for (unsigned i = 0; i < ordered_linked_list_size(musics); i++)
    {
        playlist_insert_music(playlist, ordered_linked_list_get(musics, i));
    }
}

size_t playlist_size(Playlist *playlist)
{
    ARG_ASSERT(playlist);

    if(playlist->list == NULL)
        return 0;

    return ordered_linked_list_size(playlist->list);
}

Music *playlist_get_by_order(Playlist *playlist, size_t number)
{
    ARG_ASSERT(playlist);
    OUT_OF_BOUNCE_ASSERT(number, playlist_size(playlist));

    return (Music *)ordered_linked_list_get(playlist->list, number);
}

int playlist_save_to_m3u(Playlist *playlist, char *filename)
{
    ARG_ASSERT(playlist);
    ARG_ASSERT(filename);

    FILE *file = fopen(filename, "wb");
    if(!file)
        return errno;

    fprintf(file, "#EXTM3U\r\n");
    fprintf(file, "#PLAYLIST:%s\r\n", playlist->name);

    size_t len = ordered_linked_list_size(playlist->list);
    for(size_t i = 0; i < len; ++i)
    {
        Music *music = (Music*)ordered_linked_list_get(playlist->list, i);
        fprintf(file, "#EXTINF:0,%s\r\n", music->name);
        fprintf(file, "%s\r\n", music->filename);
    }

    fclose(file);

    return 0;
}
