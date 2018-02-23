#ifndef __SHELTER_H__
#define __SHELTER_H__

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \param char* binName bin name
 * \return void
 */
void printUsage(char* binName);

/**
 *  Return a copy of the string of the command in parameter
 * 
 * \see man 3 strndup
 * \see man 3 perror
 * \return char* copied string of the command in parameter
 */
char* dupOptargStr();

/**
 *  Run the program in batch mode
 * 
 * \param char* commandParam parameter for the command
 * \return void
 */
void executeBatch(char* commandParam);

/**
 * Run the progam in shell mode
 * 
 * \param bool verbose print command if true
 * \return void
 */
void executeShell(bool verbose);

char *getHashMapValue(ForkMap *forkMap[], int index);

/**
 * Trim the Ampersand at the end of the string
 * \param char *str
 * \return char * without the ampersand at the end
 */
char *trimAmpersand(char *str);

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

void writeToFile();
void getCmdNum();

int linux_getch(void);

#endif /** __SHELTER_H__ **/