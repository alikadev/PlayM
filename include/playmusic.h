#ifndef PLAY_MUSIC_H
#define PLAY_MUSIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/* =========================== */
// The music side of playmusic
/* =========================== */

typedef struct {
	Mix_Music *sample;
	char *filename;
} Music;

/**
 * Initialize the audio device
 */
void init_audio_device(void);

/**
 * Get of the duration of a music
 * @param music The music
 * @return      Temps en seconds
 */
double music_duration(Music music);

/**
 * Stop the audio device
 */
void stop_audio_device(void);

/**
 * Load a music from a filename.
 * The music will be allocated and need to be free with free_music().
 * @param music    The pointer to the music.
 * @param filename The path of the music file
 */
void load_music(Music *music, char *filename);

/**
 * Free a music
 * @param music The music
 */
void free_music(Music music);

/**
 * Pause the music stream.
 */
void pause_music(void);

/**
 * Resume the music stream.
 */
void resume_music(void);

/**
 * Seek the music position in seconds
 * @param seconds The number of seconds
 */
void seek_music(double seconds);

/**
 * Set the music volume
 * @param volume The volume from 0 to 1
 */
void set_volume(float volume);

/**
 * Play a music.
 * @param music The music
 */
void play_music(Music music);

/**
 * Stop playing music
 */
void stop_music(void);

/**
 * Play the next music
 */
void next_music(void);

/**
 * Play the previous music
 */
void previous_music(void);

void insert_music(char *filename);

void insert_playlist_in_dir(char *dirname, char *ext);


/* ================================ */
// The processing side of playmusic
/* ================================ */

typedef void (*FunctionProcessor)(char *request);

void process_none(char *request);
void process_unknown(char *request);
void process_quit(char *request);
void process_help(char *request);
void process_resume(char *request);
void process_pause(char *request);
void process_volume(char *request);
void process_set_time(char *request);
void process_next_music(char *request);
void process_previous_music(char *request);
void process_start(char *request);
void process_start_random(char *request);
void process_load_music(char *request);
void process_load_music_directory(char *request);
void process_list(char *request);
void process_info(char *request);


/* ======================== */
// The io side of playmusic
/* ======================== */

// Colors
#define COL_AR "\e[33m"
#define COL_FN "\e[34;1m"
#define COL_NO "\e[0m"

// Standard input size
#define INPUT_SIZE 512

// Debug logging
#ifdef DEBUG
  #define debug(a ...) printf("[DEBUG] " a)
#else // NOT DEBUG
  #define debug(...)
#endif // DEBUG

// Print function
#define printfunc(fn, a1, a2)    \
printf(" "  COL_FN " %-7s"       \
	        COL_AR " %2s  %2s  " \
	        COL_NO ": %s\n",     \
	        func_name[fn],       \
	        a1, a2,              \
	        func_desc[fn])

// Print function argument
#define printfarg(arg, desc)     \
printf(" "  COL_FN " %-7s"       \
	        COL_AR " %2s  %2s  " \
	        COL_NO ": `%s` %s\n",\
	        "",                  \
	        "", "",              \
	        arg, desc)


/* ============================== */
// The function side of playmusic
/* ============================== */

typedef enum {
	FN_NONE = 0,         // No function, ignore
	FN_QUIT,             // Quit the application
	FN_HELP,             // Print the help message
	FN_UNKNOWN = 0xFF,   // Unknown function, report error
	FN_RESUME,           // Resume the audio
	FN_PAUSE,            // Pause the audio
	FN_VOLUME,           // Set the audio volume
	FN_NEXT,             // Play the next music
	FN_PREVIOUS,         // Play the previous music
	FN_SET_TIME,         // Set the music position in second
	FN_START,            // Start the audio playlist
	FN_START_RAND,       // Start the audio playlist randomly
	FN_LOAD_MUSIC,       // Load a single music
	FN_LOAD_MUSIC_DIR,   // Load a directory of music
	FN_LIST,             // Print the playlist content
	FN_INFO,             // Print informations about the music
} Function;

Function str_to_function(char *functionName);

/* =========================== */
// The logic side of playmusic
/* =========================== */

/**
 * Random value between the min and max
 * @param  min The minimal value
 * @param  max The maximal value
 * @return     The result
 */
int rand_between(int min, int max);

/**
 * Quicksort the playlist
 * @param low  The minimal value (0)
 * @param high The maximal value (music_count - 1)
 */
void quickSort(int low, int high);


#endif // PLAY_MUSIC_H