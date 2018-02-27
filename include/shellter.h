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

/**
 * Fill the forMap in param with the pid and value
 * 
 * \param ForkMap **forkMapValues contains all the values
 * \param int pid
 * \param char *value
 * \return void
 */
void FillForkMapValues(ForkMap **forkMapValues, int pid, char *value);

/**
 * Fill the first forkMap with the value of the second
 * \param ForkMap **forkMap
 * \param ForkMap **forkMapValues
 * \return void
 */
void ManageForkMap(ForkMap **forkMap, ForkMap **forkMapValues);

/**
 * Print all the fork that finished
 * \param ForkMap **forkMap
 * \return void
 */
void printForkMap(ForkMap **forkMap);

/**
 * Free the forkMap at the positon
 * 
 * \param int position
 * \return void
 */
void freeForkMap(ForkMap **forkmap, int position);

/**
 * Trim the Ampersand at the end of the string
 * \param char *str
 * \return char * without the ampersand at the end
 */
char *trimAmpersand(char *str);

#endif /** __SHELTER_H__ **/