#include <pm/audio/playlist.h>

#include <assert.h> // assert()
#include <string.h> // strerror()
#include <errno.h> // errno
#include <stdlib.h> // malloc(), free()

extern int errno;

Playlist *playlist_create(const char *name)
{
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
	for (unsigned i = 0; i < ordered_linked_list_size(musics); i++)
	{
		playlist_insert_music(playlist, ordered_linked_list_get(musics, i));
	}
}

size_t playlist_size(Playlist *playlist)
{
	if(!playlist)
		assert(0 && "Bad arg: playlist_size 'playlist' argument is NULL");

	if(playlist->list == NULL)
		return 0;

	return ordered_linked_list_size(playlist->list);
}

Music *playlist_get_by_order(Playlist *playlist, size_t number)
{
	if(playlist_size(playlist) <= number)
		assert(0 && "Out of bounce: playlist_size 'number' is greater than playlist size");

	return (Music *)ordered_linked_list_get(playlist->list, number);
}