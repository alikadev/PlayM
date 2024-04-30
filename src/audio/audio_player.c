#include <pm/audio/audio_player.h>
#include "audio_player_internal.h"
#include <pm/debug.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <assert.h> // assert()

static AudioPlayer audioPlayer;

void audio_player_initialize(void)
{
    debugfn();
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_FLAC
           | MIX_INIT_MOD
           | MIX_INIT_MP3
           | MIX_INIT_OGG
           | MIX_INIT_MID
           | MIX_INIT_OPUS);
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0)
        assert(0 && SDL_GetError());

    if (Mix_AllocateChannels(1) < 0)
        assert(0 && SDL_GetError());

    audioPlayer.playlist = NULL;
    audioPlayer.currentMusic = 0;
    audioPlayer.state |= AP_STATE_READY;

    Mix_HookMusicFinished(audio_player_play_next);
}

void audio_player_terminate(void)
{
    debugfn();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    audioPlayer.state &= ~AP_STATE_READY;
}

void audio_player_attach_playlist(Playlist *playlist)
{
    debugfn();
    if(!playlist)
        assert(0 && "Bad arg: audio_player_attach_playlist 'playlist' argument is NULL");

    audioPlayer.playlist = playlist;
}

void audio_player_detach_playlist(void)
{
    debugfn();
    audioPlayer.playlist = NULL;
}

void audio_player_play_music(void)
{
    debugfn();
    Music *music = playlist_get_by_order(audioPlayer.playlist, audioPlayer.currentMusic);
    debug("MUSIC = %s\n", music->filename);
    if (Mix_PlayMusic(music->sample, 0) < 0)
        assert(0 && SDL_GetError());
}

void audio_player_play(void)
{
    debugfn();
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
    debugfn();
    if(!audio_player_is_ready())
        assert(0 && "Internal error: audio_player_play_first audioPlayer is not ready");

    if(!audioPlayer.playlist)
        assert(0 && "Internal error: audio_player_play_first playlist is not attached");

    audioPlayer.currentMusic = 0;
    audio_player_play_music();
}

void audio_player_play_next(void)
{
    debugfn();
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
    debugfn();
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
    debugfn();
    Mix_HaltMusic();
}

void audio_player_pause(void)
{
    debugfn();
    Mix_PauseMusic();
}

void  audio_player_seek(double seconds)
{
    debugfn();
    Mix_SetMusicPosition(seconds);
}

void  audio_player_set_volume(int volume)
{
    debugfn();
    Mix_VolumeMusic(volume);
}

bool audio_player_is_ready(void)
{
    debugfn();
    return audioPlayer.state & AP_STATE_READY;
}

bool audio_player_is_playlist_attached(void)
{
    debugfn();
    return audioPlayer.playlist;
}

bool audio_player_is_playing(void)
{
    debugfn();
    return Mix_PlayingMusic();
}

bool audio_player_is_paused(void)
{
    debugfn();
    return Mix_PausedMusic();
}

size_t audio_player_current_music_id(void)
{
    debugfn();
    return audioPlayer.currentMusic;
}
