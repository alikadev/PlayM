#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <pm/audio/playlist.h>
#include <pm/debug.h>

#include <SDL2/SDL.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

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

int playlist_save_m3u(Playlist *playlist, char *filename)
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

Playlist * playlist_load_m3u(char *filename)
{
	FILE *file = fopen(filename, "r");
	if(!file)
		return NULL;

	char *line = NULL;
	size_t len;
	ssize_t read;

	Playlist *pl = playlist_create("LOADED PLAYLIST");
	
	char title[512] = {0};

	while ((read = getline(&line, &len, file)) != -1)
	{
		// Remove newline
		char *end = strchr(line, '\r');
		if (end) *end = 0;
		end = strchr(line, '\n');
		if (end) *end = 0;
		debug("LINE [%s]\n", line);

		// Is keyword? 
		if(*line == '#')
		{
			char *cmd = line+1;
			// Header, ignore
			if(strcmp(cmd, "EXTM3U") == 0)
				continue;

			// Get everything after ':'
			char* next = strchr(cmd, ':');
			if(next && *next)
			{
				*next = 0;
				next++;

				// Playlist name
				if(strcmp(cmd, "PLAYLIST") == 0)
				{
					char *newName = malloc(strlen(next) + 1);
					if(!newName)
					{
						printf("Fail to get playlist name: %s\n", strerror(errno));
						continue;
					}
					strcpy(newName, next);
					free(pl->name);
					pl->name = newName;
					continue;
				}

				// Informations about music (title, ...)
				if(strcmp(cmd, "EXTINF") == 0)
				{
					next = strchr(next, ',');
					if (next && next + 1)
						strcpy(title, next + 1);
					continue;
				}
			}
		} else if (*line) {
			// Music
			Music *music = music_load_from_file(line);
			if (!music)
			{
				printf("%s cannot be loaded: %s\n", line, SDL_GetError());
				continue;
			}

			// Music title
			if (*title)
			{
				char *newTitle = malloc(sizeof(title) + 1);
				if(!newTitle)
				{
					printf("Fail to get music title: %s\n", strerror(errno));
					continue;
				}
				strcpy(newTitle, title);
				free(music->name);
				music->name = newTitle;
			}
			*title = 0;

			// Insert music...
			playlist_insert_music(pl, music);
			continue;
		}
	}

	fclose(file);

	if(line) 
		free(line);

	return pl;
}