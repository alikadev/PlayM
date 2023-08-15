#ifndef PLAYMUSIC_STRING_H
#define PLAYMUSIC_STRING_H

char *ignore_spaces(char *str);

/**
 * Return the next token (separated by space) or NULL if there's no other token
 * @param  str The input string
 * @return     The next token address
 */
char *next_token(char *str);

#endif