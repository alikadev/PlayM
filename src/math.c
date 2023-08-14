#include <playmusic.h>

extern Music *playlist;

/**
 * Random value between the min and max
 * @param  min The minimal value
 * @param  max The maximal value
 * @return     The result
 */
int rand_between(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

// Source: https://www.geeksforgeeks.org/quick-sort/
// Function to swap two elements
void swap_musics(Music* a, Music* b)
{
    char *aFilename = a->filename;
    Mix_Music *aSample = a->sample;

    a->filename = b->filename;
    a->sample = b->sample;

    b->filename = aFilename;
    b->sample = aSample;
}
 
// Partition the array using the last element as the pivot
int quickSort_partition(int low, int high)
{
    // Choosing the pivot
    Music pivot = playlist[high];
 
    // Index of smaller element and indicates
    // the right position of pivot found so far
    int i = (low - 1);
 
    for (int j = low; j <= high - 1; j++) {
 
        // If current element is smaller than the pivot
        if (strcmp(playlist[j].filename, pivot.filename) < 0) {
 
            // Increment index of smaller element
            i++;
            swap_musics(&playlist[i], &playlist[j]);
        }
    }
    swap_musics(&playlist[i + 1], &playlist[high]);
    return (i + 1);
}

/**
 * Quicksort the playlist
 * @param low  The minimal value (0)
 * @param high The maximal value (music_count - 1)
 */
void quickSort(int low, int high)
{
    if (low < high) {
 
        // pi is partitioning index, arr[p]
        // is now at right place
        int pi = quickSort_partition(low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(low, pi - 1);
        quickSort(pi + 1, high);
    }
}