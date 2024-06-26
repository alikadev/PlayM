#include <pm/sys.h>
#include <pm/sys/asserts.h>
#include <ctype.h>
#include <stddef.h>

char *ignore_spaces(char *str)
{
    ARG_ASSERT(str);
    while (isspace(*str))
        str++;
    return str;
}

/**
 * Return the next token (separated by space) or NULL if there's no other token
 * @param  str The input string
 * @return     The next token address
 */
char *next_token(char *str)
{
    ARG_ASSERT(str);
    while (*str)
    {
        if (!isspace(*str))
        {
            str++;
            continue;
        }
        str = ignore_spaces(str);
        return str;
    }
    return NULL;
}
