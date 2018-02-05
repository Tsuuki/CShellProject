/**
 * Execute the command in the node tree
 * \param Node* rootNode of the tree
 */
void execute(struct Node*);

/**
 * Check if there is a  build in command
 * \param Node* rootNode of the tree
 */
void checkBuildInCommand(struct Node*);

/**
 * Change directory to the path
 * \param char* path new path
 */
void changeDirectory(char*);

/**
 * Print the current working directory
 */
void printWorkingDirectory();

/**
 * Print the text in the shell
 * \param char* text text to print
 */
void echo(char*);

/**
 * Exit the shell by passing run to false
 */
void exitShell();

/**
 * trim the double quote at start and end of a string
 * \param str the string
 * \return str the string without starting and ending double quote
 */
char* trimDoubleQuote(char* str);