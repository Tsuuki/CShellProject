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
 * \return void
 */
void printHistory(char *search, int commandNumber);

/**
 * Print the given command get in history
 * \param char *command the given command
 * \return void
 */
void printHistoryCommand(char *command); 

/**
 * Read history to the top or bot according to param way
 * 
 * \param int way 0 --> top 1 --> bot
 * \param char **str address to string command
 * \return void 
 */
void readHistory(int way, char **str);

/**
 * Procedure that reset history command counter to 0
 * \param int commandNumber the last command number
 * \return void
 */ 
void resetHistoryCounter(int commandNumber);
#endif /** __HISTORY_H__ **/