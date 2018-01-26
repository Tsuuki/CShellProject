#include <stdio.h>

/**
 * Clean the char* get by fgets
 * \param const char* buffer to clean
 * \param FILE stream to get the next buffer
 */
void clean(const char*, FILE*);

/**
 * Retrieve user name executing program
 * \return char*
 */ 
char* getUserName();

/**
 * Retrieve user host name
 * \return char*
 */
char* getUserHostName();

/**
 * Replace substring from another within string
 * \param char* original string
 * \param char* to replace
 * \param char* with
 * \return char* new string replaced
 */ 
char* str_replace(char*, char*, char*);

/**
 * Check if the program is execute with root permission
 * \return char* corresponding to root or not
 */ 
char* getRootPermission();