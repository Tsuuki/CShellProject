/**
 *Execute the command in the node
 *
 *\param Node *node to be executed
 *\return void
 */
void execute(struct Node *node);

/**
 *Check if there is a  build in command
 *
 *\param Node *node of the tree to check
 *\return bool true if there was a buildInCommand
 */
bool checkBuildInCommand(struct Node *node);

/**
 *Execute the command in parameter
 *
 *\param struct Node *node containing the command to execute
 *\return bool true if a command has been executed
 */
bool executeCommand(struct Node *node);

/**
 *Change directory to the path
 *
 *\param char *new path
 *\return void
 */
void changeDirectory(char *path);

/**
 *Print the current working directory
 *
 *\return void
 */
void printWorkingDirectory();

/**
 *Print the string in the shell
 *
 *\param char *str to print
 *\return void
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
 *Trim the double quote at start and end of a string
 *
 *\param str the string
 *\return str the string without starting and ending double quote
 */
char *trimDoubleQuote(char *str);

/**
 *Fill the action array
 *
 *\param char** *action array pointer
 *\param char *command 
 *\param char *arguments
 *\return void
 */
void fillActionArray(char ***action, char *command, char *arguments);