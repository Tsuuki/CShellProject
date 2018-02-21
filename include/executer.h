#ifndef __EXECUTER_H__
#define __EXECUTER_H__

/**
 * Execute the command in the node
 *
 * \param Node *node to be executed
 * \param bool true if it nedd to be fork
 */
void execute(Node *node, bool isForked);

/**
 * Check if there is a  build in command
 *
 * \param Node *node of the tree to check
 * \return bool true if there was a buildInCommand
 */
bool checkBuildInCommand(Node **node);

/**
 * Check if the command has been executed with the code in parameter
 * 
 * \param Node *node to print errors
 * \param int code
 * \return bool true if there was a command executed 
 */
bool checkResult(Node *node, int code);

/**
 * Execute the command without fork
 * 
 * \param Node *node containing the command to execute
 * \return int code 0 : ok 1 : error in function 2 : unknown function
 */
int executeCommand(Node *node);

/**
 * Execute the command in parameter with fork
 *
 * \param Node *node containing the command to execute
 * \return int code 0 : ok 1 : error in function 2 : unknown function
 */
int executeCommandForked(Node *node);

/**
 * Change directory to the path
 *
 * \param char *new path
 * \return void
 */
void changeDirectory(char *path);

/**
 * Print the current working directory
 *
 * \return void
 */
void printWorkingDirectory();

/**
 * Print the string in the shell
 *
 * \param char *str to print
 * \return void
 */
void echo(char *str);

/**
 * Print the current history of shell
 * 
 * \return void
 */
void printHistory();

/**
 * Exit the shell by passing run to false
 * 
 * \return void
 */
void exitShell();

/**
 * Trim the double quote at start and end of a string
 *
 * \param str the string
 * \return str the string without starting and ending double quote
 */
char *trimDoubleQuote(char *str);

/**
 * Fill the action array
 *
 * \param char** *action array pointer
 * \param char *command 
 * \param char *arguments
 * \return void
 */
void fillActionArray(char ***action, char *command, char *arguments);

/**
 * Get the size needed for the tmp array
 * 
 * \param char *arguments of the action
 * \return int size
 */
int getSize(char *arguments);

#endif /* __EXECUTER_H__ */