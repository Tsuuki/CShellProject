#ifndef __REGEX_H__
#define __REGEX_H__

/**
 * Check if a string match a specify regex
 * \param const char* regex
 * \param char** address to string which will contain the match
 * \param char* the given string
 */
bool checkRegex(const char*, char**, char*);

#endif /** __REGEX_H__ */