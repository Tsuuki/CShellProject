/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \return void
 */
void printUsage(char*);

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
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
char* dupOptargStr();

/**
 *  Run the program in batch mode
 * 
 * \param char* commandParam parameter for the command
 */
void executeBatch(char*);

/**
 * Run the progam in shell mode
 */
void executeShell();

/**
 * Display shell prompt at screen
 */
void printPrompt();

/**
 * Get the current users directory
 * \return char*
 */
char* getWorkingDirectory();

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
const char* getUserName();
