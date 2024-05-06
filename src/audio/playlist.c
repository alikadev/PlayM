#include <pm/audio/playlist.h>
#include <pm/sys/asserts.h>

#include <string.h>
#include <errno.h> 
#include <stdio.h>
#include <stdlib.h>
#include <wordexp.h>

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

    // Get `real path`
    wordexp_t result;
    wordexp(filename, &result, 0);
    const char *real_path = result.we_wordv[0];
    if (!real_path)
    {
        fprintf(stderr, "Path must not be empty\n");
        return ENOENT;
    }

    FILE *file = fopen(real_path, "wb");
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

    fprintf(file, "\r\n");

    fclose(file);

    return 0;
}

int playlist_load_m3u(Playlist **p_plist, char *filename)
{
    ARG_ASSERT(p_plist);
    ARG_ASSERT(filename);

    int status = 0;
    char buff[512] = {0};
    Playlist *plist;

    // Get `real path`
    wordexp_t result;
    wordexp(filename, &result, 0);
    const char *real_path = result.we_wordv[0];
    if (!real_path)
        return ENOENT;

    // Open file
    FILE *file = fopen(real_path, "rb");
    if (!file)
        return errno;

    // Check M3U header
    fgets(buff, 512, file);
    if (memcmp("#EXTM3U", buff, strlen("#EXTM3U")) != 0)
        return EFTYPE;

    // Create and fill output playlist
    plist = playlist_create("Unnamed");
    while (!feof(file))
    {
        fgets(buff, 512, file);
        printf("%s", buff);
        if (*buff == '\r' || *buff == '\n' || !*buff)
            continue;
        else if (memcmp("#PLAYLIST", buff, strlen("#PLAYLIST")) == 0)
        {
            // Skip `#PLAYLIST:`
            char *it = buff + strlen("#PLAYLIST") + 1;
            char *end = strchr(buff, '\r');
            if (!end) end = strchr(buff, '\n');
            if (!end) end = buff + strlen(buff) + 1;
            if (end) *end = '\0';
            // Rename playlist with new name
            free(plist->name);
            plist->name = malloc(strlen(it) + 1);
            ALLOC_ASSERT(plist->name);
            strcpy(plist->name, it);
            continue;
        }
        else if (memcmp("#EXTINF", buff, strlen("#EXTINF")) == 0)
        {
            // TODO: Add suport for track name vs track path
            // Read music path
            if (feof(file)) { status = EFTYPE; goto failure; }
            fgets(buff, 512, file);
 
            // Replace CR/LF/NULL = \0
            char *end = strchr(buff, '\r');
            if (!end) end = strchr(buff, '\n');
            if (!end) end = buff + strlen(buff) + 1;
            if (end) *end = '\0';

            // Create music
            printf("Buff: %s\n", buff);
            Music *m = music_load_from_file(buff);
            if (!m) { status = ENOENT; goto failure; }
            playlist_insert_music(plist, m);
            if (feof(file)) break;
            continue;
        }
        else 
        {
            printf("Unexpected: %s\n", buff);
            status = EFTYPE;
            goto failure;
        }
    }

    *p_plist = plist;
    return 0;

failure:
    playlist_destroy(plist);
    return status;
}
