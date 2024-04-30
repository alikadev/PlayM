#include <pm/audio/playlist.h>
#include <pm/debug.h>

#include <assert.h> // assert()
#include <string.h> // strerror()
#include <errno.h> // errno
#include <stdlib.h> // malloc(), free()

extern int errno;

Playlist *playlist_create(const char *name)
{
    debugfn();
    Playlist *playlist = malloc(sizeof *playlist);
    if(!playlist)
        assert(0 && strerror(errno));

    playlist->name = malloc(strlen(name) + 1);
    if(!playlist->name)
        assert(0 && strerror(errno));
    strcpy(playlist->name, name);

    playlist->list = NULL;
    return playlist;
}

void playlist_destroy(Playlist *playlist)
{
    debugfn();
    if(!playlist)
        assert(0 && "Bad arg: playlist_destroy 'playlist' argument is NULL");

    if(playlist->name)
        free(playlist->name);

    if(playlist->list)
        ordered_linked_list_destroy(playlist->list);

    free(playlist);
}

void playlist_insert_music(Playlist *playlist, Music *music)
{
    debugfn();
    if(!playlist)
        assert(0 && "Bad arg: playlist_insert_music 'playlist' argument is NULL");

    if(!music)
        assert(0 && "Bad arg: playlist_insert_music 'music' argument is NULL");

    if(!playlist->list)
    {
        playlist->list = ordered_linked_list_create(music, (int(*)(void*,void*))music_compare);
    } else {
        ordered_linked_list_insert(&playlist->list, music);
    }
}

void playlist_insert_music_list(Playlist *playlist, OrderedLinkedList *musics)
{
    debugfn();    
    for (unsigned i = 0; i < ordered_linked_list_size(musics); i++)
    {
        playlist_insert_music(playlist, ordered_linked_list_get(musics, i));
    }
}

size_t playlist_size(Playlist *playlist)
{
    debugfn();
    if(!playlist)
        assert(0 && "Bad arg: playlist_size 'playlist' argument is NULL");

    if(playlist->list == NULL)
        return 0;

    return ordered_linked_list_size(playlist->list);
}

Music *playlist_get_by_order(Playlist *playlist, size_t number)
{
    debugfn();
    if(playlist_size(playlist) <= number)
        assert(0 && "Out of bounce: playlist_size 'number' is greater than playlist size");

    return (Music *)ordered_linked_list_get(playlist->list, number);
}

int playlist_save_to_m3u(Playlist *playlist, char *filename)
{
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