#define _GNU_SOURCE

#include <pm/audio/music.h>
#include <pm/sys/asserts.h>

#include <errno.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <errno.h>
#include <string.h> 
#include <wordexp.h>

extern int errno;

/**
 * Load a music from a filename.
 * The music will be allocated and need to be free with free_music().
 * @param music    The pointer to the music.
 * @param filename The path of the music file
 */
Music *music_load_from_file(char *filename)
{
    ARG_ASSERT(filename);

    // Get real path
    wordexp_t result;
    wordexp(filename, &result, 0);
    const char *real_path = result.we_wordv[0];
    if (!real_path)
    {
        fprintf(stderr, "Path must not be empty\n");
        return NULL;
    }
    
    // Allocate 
    Music *music = malloc(sizeof *music);
    if (!music)
        return NULL;

    // Load music
    music->sample = Mix_LoadMUS(real_path);
    if(!music->sample)
    {
        printf("Fail to load the music: %s\n", SDL_GetError());
        free(music);
        return NULL;
    }
    
    // Copy filename
    music->filename = malloc(strlen(real_path) + 1);
    if(!music->filename)
    {
        printf("Fail to allocate the music: %s\n", strerror(errno));
        Mix_FreeMusic(music->sample);
        free(music);
        return NULL;
    }
    strcpy(music->filename, filename);
    
    // Copy music name (filename without the extension)
    *strrchr(filename, '.') = '\0';
    music->name = malloc(strlen(filename) + 1);
    if(!music->filename)
    {
        printf("Fail to allocate the name: %s\n", strerror(errno));
        Mix_FreeMusic(music->sample);
        free(music->filename);
        free(music);
        return NULL;
    }
    strcpy(music->name, real_path);

    return music;
}

OrderedLinkedList *music_load_directory(char *dirname, char *ext)
{
    ARG_ASSERT(dirname);

    // Get real path
    wordexp_t result;
    wordexp(dirname, &result, 0);
    const char *real_path = result.we_wordv[0];
    if (!real_path)
    {
        fprintf(stderr, "Path must not be empty\n");
        return NULL;
    }

    // Load directory
    OrderedLinkedList *musicList = NULL;
    DIR *d;
    struct dirent *dir;
    d = opendir(real_path);
    if (!d)
    {
        printf("  Fail to open the directoy: %s\n", strerror(errno));
        return NULL;
    }

    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type != DT_REG)
            continue;
        char *dot = strrchr(dir->d_name, '.');
        if ((dot && (strcmp(dot+1, ext) == 0)) || strcmp(ext, "*") == 0)
        {
            char *filename = malloc(strlen(real_path) + strlen(dir->d_name) + 2);
            // TODO: Find a better way to handle this
            ALLOC_ASSERT(filename); 
            strcpy(filename, real_path);
            strcat(filename, "/");
            strcat(filename, dir->d_name);
            printf("  Loading %s\n", filename);
            Music *music = music_load_from_file(filename);
            if (!music)
            {
                printf("Fail to load music: %s\n", strerror(errno));
                free(filename);
                return NULL;
            }
            if(musicList)
                ordered_linked_list_insert(&musicList, music);
            else
                musicList = ordered_linked_list_create(music, (int(*)(void*,void*))music_compare);
            free(filename);
        }
    }

    closedir(d);
    return musicList;
}

void music_unload(Music *music)
{
    ARG_ASSERT(music);
    Mix_FreeMusic(music->sample);
    free(music->filename);
    free(music);
}

void music_unload_directory(OrderedLinkedList *musicList)
{
    ARG_ASSERT(musicList);
    OrderedLinkedList *node = musicList;
    while(node)
    {
        music_unload(node->elem);
        node = node->next;
    }

    ordered_linked_list_destroy(musicList);
}

int music_compare(Music *m1, Music *m2)
{
    ARG_ASSERT(m1);
    ARG_ASSERT(m2);
    return strcmp(m1->filename, m2->filename);
}
