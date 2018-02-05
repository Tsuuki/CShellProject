/**
 * Execute the command in the node tree
 * 
 * \param Node* rootNode of the tree
 * \return void
 */
void execute(struct Node* rootNode);

/**
 * Check if there is a  build in command
 * 
 * \param Node* node of the tree to check
 * \return bool true if there was a buildInCommand
 */
bool checkBuildInCommand(struct Node* node);

/**
 * Execute the command in parameter
 * 
 * \param struct Node* node containing the command to execute
 * \return void
 */
void executeCommand(struct Node* node);

/**
 * Change directory to the path
 * 
 * \param char* new path
 * \return void
 */
void changeDirectory(char* path);

/**
 * Print the current working directory
 * 
 * \return void
 */
void printWorkingDirectory();

/**
 * Print the string in the shell
 * 
 * \param char* str to print
 * \return void
 */
void echo(char* str);

/**
 * Exit the shell by passing run to false
 * \return void
 */
void exitShell();

/**
 * trim the double quote at start and end of a string
 * \param str the string
 * \return str the string without starting and ending double quote
 */
char* trimDoubleQuote(char* str);

char* fillArray(char* command, char* arguments);