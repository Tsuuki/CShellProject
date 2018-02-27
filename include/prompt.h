#ifndef __PROMPT_H__
#define __PROMPT_H__

/**
 * Print a welcome message
 * 
 * \return void
 */
void printWelcome();

/**
 * Display shell prompt at screen
 * 
 * \return char* command enter by the user
 */
char* prompt();

/**
 * Function that return a char code typed on keyboard
 * 
 * \return int code corresponding to char
 * 
 */ 
int linux_getch();

/**
 * Procedure that move cursor in console
 * 
 * \param int way 0 for left 1 for right
 * \param int length the number of move to do
 * 
 * \return void
 */ 
void moveCursor(int way, int length);

/**
 * Procedure that remove all char from console
 * 
 * \param int nbCharToDelete number char to delete
 * 
 * \return void
 */ 
void clearPrompt(int nbCharToDelete);


#endif /** __PROMPT_H__ **/