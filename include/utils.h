#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * Check if the program is execute with root permission
 * \return char* corresponding to root or not
 */ 
char* getRootPermission();

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
 * Get the current users directory
 * \return char*
 */
char* getWorkingDirectory();

/**
 * Replace substring from another within string
 * \param char* original string
 * \param char* to replace
 * \param char* with
 * \return char* new string replaced
 */ 
char* str_replace(char*, char*, char*);

/**
 * Clean the char* get by fgets
 * \param const char* buffer to clean
 * \param FILE stream to get the next buffer
 */
void clean(const char*, FILE*);

/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* toFree pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void freeIfNeeded(void*);

/**
 * Procedure that clean a char* with 0x00
 * 
 * \param char *str the string to clean
 * \param int size the size of string
 * 
 * \return void
 * 
 */ 
void clearStr(char* str, int size);

/**
 * Procedure that remove a char from a string at specific position
 * And then, change index of every char after
 * 
 * \param int position the char position to remove
 * \param char **str a pointer to the string
 * 
 */ 
void removeCharString(int position, char **str);

/**
 * Procedure that add a char in string at specific position
 * and move other char
 * 
 * \param int kb_char specific char
 * \param int cursor_position the position
 * \param char **str the address of string
 * 
 * \return void
 * 
 */
void writeToString(int kb_char, int cursor_position, char **str); 
#endif /** __UTILS_H__ **/