#ifndef PLAYMUSIC_H
#define PLAYMUSIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <pm/audio.h>
#include <pm/io.h>
#include <pm/list.h>
#include <pm/string.h>

/* ================================ */
// The processing side of playmusic
/* ================================ */

typedef void (*FunctionProcessor)(char *request);

void process_none(char *request);
void process_unknown(char *request);
void process_quit(char *request);
void process_help(char *request);
void process_play(char *request);
void process_pause(char *request);
void process_volume(char *request);
void process_set_time(char *request);
void process_next_music(char *request);
void process_previous_music(char *request);
void process_start(char *request);
void process_load_music(char *request);
void process_load_music_directory(char *request);
void process_playlist(char *request);
void process_music(char *request);


/* ======================== */
// The io side of playmusic
/* ======================== */

// Standard input size
#define INPUT_SIZE 512

// Print function
#define printfunc(fn, a1, a2) \
printf("  %-8s"               \
       " %2s  %2s  "          \
       ": %s\n",              \
       func_name[fn],         \
       a1, a2,                \
       func_desc[fn])

// Print function argument
#define printfarg(arg, desc)  \
printf("  %-8s"               \
       " %2s  %2s  "          \
       "  `%s` %s\n",         \
       "",                    \
       "", "",                \
       arg, desc)


/* ============================== */
// The function side of playmusic
/* ============================== */



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


#endif // PLAYMUSIC_H