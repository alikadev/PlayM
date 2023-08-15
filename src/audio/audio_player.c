#include <pm/audio/audio_player.h>
#include "audio_player_internal.h"
#include <pm/debug.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <assert.h> // assert()

static AudioPlayer audioPlayer;

void audio_player_initialize(void)
{
	debug("%s\n", __FUNCTION__);
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0)
    	assert(0 && SDL_GetError());

    if (Mix_AllocateChannels(4) < 0)
    	assert(0 && SDL_GetError());

    audioPlayer.playlist = NULL;
    audioPlayer.currentMusic = 0;
    audioPlayer.state |= AP_STATE_READY;

    Mix_HookMusicFinished(audio_player_play_next);
}

void audio_player_terminate(void)
{
	debug("%s\n", __FUNCTION__);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    audioPlayer.state &= ~AP_STATE_READY;
}

void audio_player_attach_playlist(Playlist *playlist)
{
	debug("%s\n", __FUNCTION__);
	if(!playlist)
		assert(0 && "Bad arg: audio_player_attach_playlist 'playlist' argument is NULL");

	audioPlayer.playlist = playlist;
}

void audio_player_detach_playlist(void)
{
	debug("%s\n", __FUNCTION__);
	audioPlayer.playlist = NULL;
}

void audio_player_play_music(void)
{
	Music *music = playlist_get_by_order(audioPlayer.playlist, audioPlayer.currentMusic);
	debug("MUSIC = %s\n", music->filename);
	if (Mix_PlayMusic(music->sample, 0) < 0)
		assert(0 && SDL_GetError());
}

void audio_player_play(void)
{
	debug("%s\n", __FUNCTION__);
	if(!audio_player_is_ready())
		assert(0 && "Internal error: audio_player_play_first musicPlayer is not ready");

	if(!audioPlayer.playlist)
		assert(0 && "Internal error: audio_player_play_first playlist is not attached");

	if(!audio_player_is_playing())
	{
		audio_player_play_music();
	} else if(audio_player_is_paused()) {
		Mix_ResumeMusic();
	}
}

void audio_player_play_first(void)
{
	debug("%s\n", __FUNCTION__);
	if(!audio_player_is_ready())
		assert(0 && "Internal error: audio_player_play_first audioPlayer is not ready");

	if(!audioPlayer.playlist)
		assert(0 && "Internal error: audio_player_play_first playlist is not attached");

	audioPlayer.currentMusic = 0;
	audio_player_play_music();
}

void audio_player_play_next(void)
{
	debug("%s\n", __FUNCTION__);
	if(!audio_player_is_ready())
		assert(0 && "Internal error: audio_player_play_first audioPlayer is not ready");

	if(!audioPlayer.playlist)
		assert(0 && "Internal error: audio_player_play_first playlist is not attached");

	audioPlayer.currentMusic++;
	audioPlayer.currentMusic %= playlist_size(audioPlayer.playlist);
	audio_player_play_music();
}

void audio_player_play_prev(void)
{
	debug("%s\n", __FUNCTION__);
	if(!audio_player_is_ready())
		assert(0 && "Internal error: audio_player_play_first audioPlayer is not ready");

	if(!audioPlayer.playlist)
		assert(0 && "Internal error: audio_player_play_first playlist is not attached");

	if(audioPlayer.currentMusic == 0)
		audioPlayer.currentMusic = playlist_size(audioPlayer.playlist);
	audioPlayer.currentMusic--;
	audio_player_play_music();
}

void audio_player_halt_music(void)
{
	debug("%s\n", __FUNCTION__);
	Mix_HaltMusic();
}

void audio_player_pause(void)
{
	debug("%s\n", __FUNCTION__);
	Mix_PauseMusic();
}

void  audio_player_seek(double seconds)
{
	Mix_SetMusicPosition(seconds);
}

void  audio_player_set_volume(float volume)
{
	Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}

bool audio_player_is_ready(void)
{
	debug("%s\n", __FUNCTION__);
	return audioPlayer.state & AP_STATE_READY;
}

bool audio_player_is_playlist_attached(void)
{
	debug("%s\n", __FUNCTION__);
	return audioPlayer.playlist;
}

bool audio_player_is_playing(void)
{
	debug("%s\n", __FUNCTION__);
	return Mix_PlayingMusic();
}

bool audio_player_is_paused(void)
{
	debug("%s\n", __FUNCTION__);
	return Mix_PausedMusic();
}

size_t audio_player_current_music_id(void)
{
	return audioPlayer.currentMusic;
}