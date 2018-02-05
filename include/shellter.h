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
 * Print a welcome message
 */
void printWelcome();

/**
 * Display shell prompt at screen
 * \return char* command enter by the user
 */
char* prompt();

void writeToFile();
