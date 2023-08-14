#define _GNU_SOURCE

#include <playmusic.h>

#include <errno.h>
#include <dirent.h> 
#include <sys/stat.h>

extern Music *playlist;
extern size_t current_music;
extern size_t music_count;

/**
 * Initialize the audio device
 */
void init_audio_device(void)
{
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0)
    {
    	printf("Fail to open the audio device: %s\n", SDL_GetError());
    	exit(EXIT_FAILURE);
    }

    if (Mix_AllocateChannels(4) < 0)
    {
    	printf("Fail to allocate channels: %s\n", SDL_GetError());
    }

    Mix_HookMusicFinished(next_music);
}

/**
 * Stop the audio device
 */
void stop_audio_device(void)
{
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

/**
 * Load a music from a filename.
 * The music will be allocated and need to be free with free_music().
 * @param music    The pointer to the music.
 * @param filename The path of the music file
 */
void load_music(Music *music, char *filename)
{
	music->sample = Mix_LoadMUS(filename);
	music->filename = malloc(strlen(filename) + 1);
	if(!music->filename)
	{
		printf("Fail to allocate the music: %s\n", strerror(errno));
	}
	strcpy(music->filename, filename);
}

/**
 * Free a music
 * @param music The music
 */
void free_music(Music music)
{
	Mix_FreeMusic(music.sample);
	free(music.filename);
}

/**
 * Pause the music stream.
 */
void pause_music(void)
{
	if (!Mix_PausedMusic())
		Mix_PauseMusic();
}

/**
 * Resume the music stream.
 */
void resume_music(void)
{
	if (Mix_PausedMusic())
		Mix_ResumeMusic();
}

/**
 * Seek the music position in seconds
 * @param seconds The number of seconds
 */
void seek_music(double seconds)
{
	Mix_SetMusicPosition(seconds);
}

/**
 * Set the music volume
 * @param volume The volume from 0 to 1
 */
void set_volume(float volume)
{
	Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}

/**
 * Play a music.
 * @param music The music
 */
void play_music(Music music)
{
	if (Mix_PlayMusic(music.sample, 0) < 0)
	{
		printf("Fail to play music: %s\n", SDL_GetError());
		return;
	}
	printf("\rPlaying %s\n", music.filename);
}

void stop_music(void)
{
	Mix_HaltMusic();
}

/**
 * Play the next music
 */
void next_music(void)
{
	current_music = (current_music + 1) % music_count;
	play_music(playlist[current_music]);
}

/**
 * Play the previous music
 */
void previous_music(void)
{
	if (current_music == 0)
		current_music = music_count - 1;
	else
		current_music -= 1;
	play_music(playlist[current_music]);
}

void insert_music(char *filename)
{
	Music *tmp = realloc(playlist, sizeof(Music) * (music_count + 1));
	if (tmp == NULL)
	{
		printf("Fail to load a music: %s\n", strerror(errno));
		return;
	}
	playlist = tmp;

	load_music(&playlist[music_count], filename);
	music_count++;

	quickSort(0, music_count - 1);
}

void insert_playlist_in_dir(char *dirname, char *ext)
{
	DIR *d;
	struct dirent *dir;
	d = opendir(dirname);
	if (!d)
	{
		printf("  Fail to open the directoy: %s\n", strerror(errno));
		return;
	}

	while ((dir = readdir(d)) != NULL) {
		if (dir->d_type != DT_REG)
			continue;
		char *dot = strrchr(dir->d_name, '.');
		if (dot && (strcmp(dot+1, ext) == 0))
		{
			char *filename = malloc(strlen(dirname) + strlen(dir->d_name) + 2);
			strcpy(filename, dirname);
			strcat(filename, "/");
			strcat(filename, dir->d_name);
			printf("  Loading %s\n", filename);
			insert_music(filename);
			free(filename);
		}
	}

	closedir(d);
}

double music_duration(Music music)
{
	(void) music;
	return -1; //Mix_MusicDuration(music.sample);
}