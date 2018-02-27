#ifndef __MANAGEALIAS_H__
#define __MANAGEALIAS_H__

/**
 * Call the right function for add print or remove aliases
 * \param Node* node containing command and arguments
 * \return void
 */ 
void manageAlias(struct Node *node);

/**
 * Add alias to the current shell
 * \param char* containing alias name and command
 * \return void
 */ 
void addAlias(char *alias);

/**
 * Delete alias to the current shell
 * \param char* alias name
 * \return void
 */
void delAlias(char *alias);

/**
 * Delete all alias to the current shell
 * \return void 
 */ 
void delAllAlias();

/**
 * Search alias from the current shell
 * \param char* alias name
 * \return void
 */
void searchAlias(char *alias);

/**
 * Remove alias to the current shell
 * \return void
 */ 
void removeAlias();

/**
 * Print all alias of the current shell
 * \return void
 */ 
void printAlias();

/**
 * Printf alias usage
 * \return void
 */ 
void printAliasUsage();

/**
 * Init an empty array of alias
 * \return void
 */
void initAliases();

/**
 * Retrieve alias name and command from a string
 * \param char* alias with name and command
 * \param char** address to string which will contain alias name
 * \param char** address to string which will contain alias command
 * \return void
 */
void getAliasInfos(char*, char**, char**);

/**
 * Check if an alias already exist
 * \param char* alias name
 * \return the index of alias or -1 if it not exist
 */
int isAliasExist(char*);

#endif /** __MANAGEALIAS_H__ **/