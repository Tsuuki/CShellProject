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
 * 
 * \return void
 */
void printHistory(char* search);
#endif /** __HISTORY_H__ **/