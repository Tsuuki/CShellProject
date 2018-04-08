#ifndef __FORK_H__
#define __FORK_H__

/**
 * Init the forkMap in param
 * 
 * \param ForkMap **forkMap to init
 * \return void
 */
void initForkMap(ForkMap **forkMap);

/**
 * Fill the forMap in param with the pid and value
 * 
 * \param ForkMap **forkMap contains all the values
 * \param int pid
 * \param char *value
 * \return void
 */
void FillForkMap(ForkMap **forkMap, int pid, char *value);

/**
 * Fill the first forkMap with the value of the second
 * \param ForkMap **forkMap copied in
 * \param ForkMap **forkMap who will be copied
 * \return void
 */
void ManageForkMap(ForkMap **forkMap1, ForkMap **forkMap2);

/**
 * Print all the fork that finished
 * \param ForkMap **forkMap
 * \return void
 */
void printForkMap(ForkMap **forkMap);

/**
 * Free the forkMap at the index
 * 
 * \param int index
 * \return void
 */
void freeForkMap(ForkMap **forkmap, int index);

/**
 * Allow to make the command at the index in foreground
 * 
 * \param int index
 * \retrun void
 */
void makeForeground(int index);

#endif /** __FORK_H__ **/