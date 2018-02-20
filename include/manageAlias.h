/**
 * Call the right function for add print or remove aliases
 * \return void
 */ 
void manageAlias(struct Node *node);

/**
 * Add alias to the current shell
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
 * Init an empty alias
 * \return void
 */
Alias initAlias();

/**
 * Retrieve alias name from command
 * \return Alias name
 */
void getAliasInfos(char*, char**, char**);

/**
 * Check if an alias already exist
 * \param char* alias name
 */
int isAliasExist(char*);

bool checkRegex(const char*, char**, char*);