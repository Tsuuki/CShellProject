#ifndef __HISTORY_H__
#define __HISTORY_H__

/**
 * Call the right function for print or research history
 * \param Node* node containing command and arguments
 * \return void
 * */
void manageHistory(Node *node);

/**
 * Print the current history of shell
 * \param char *search use for searching command in history search
 * \param int commandNumber use for printing specific command with arrow key
 * \param char ** pointer to the string to fill with command found
 * \return void
 */
void printHistory(char *search, int commandNumber, char **command);

/**
 * Function that extract the command by removing all other char
 * \param char *command the given command line
 * \return char* the command
 */
char* extractHistoryCommand(char *command); 

/**
 * Read history to the top or bot according to param way
 * 
 * \param int way 0 --> top 1 --> bot
 * \param char **str address to string command
 * \param int commandNumber the actual command number
 * \return void 
 */
void readHistory(int way, char **str, int commandNumber);

/**
 * Procedure that reset history command counter to 0
 * \param int commandNumber the last command number
 * \return void
 */ 
void resetHistoryCounter(int commandNumber);

/**
 * Function that retrieve command number from history file
 * \return int command number
 */ 
int retrieveCommandNumber();

/**
 * Function that extract command number from line command
 * \param FILE *fpHistory history file
 * \return int command number
 */ 
int getCmdNum(FILE *fpHistory);
#endif /** __HISTORY_H__ **/