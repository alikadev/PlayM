#include <pm/sys.h>
#include <stdlib.h>

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
