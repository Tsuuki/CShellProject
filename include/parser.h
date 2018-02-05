/**
 * Parse the commmand in parameter
 * 
 * \param char* command to parse
 * \return struct Node* return the command into a struct Node *
 */
struct Node* parse(char* command);

/**
 * Create and return a new struct Node from operator and command in parameters 
 * 
 * \param char* operator to add in the node
 * \param char* command to add in the node
 * \param char* arguments to add in the node
 * \return struct Node* newly created
 */
struct Node* createNode(char* operator, char* command, char* arguments);

/**
 * Add the next node to a node and return the next node
 * 
 * \param Node* node to have a next one
 * \param char* operator to add in the next node
 * \param char* command to add in the node
 * \param char* arguments to add in the node
 * \return struct Node* node created
 */
struct Node* addNode(struct Node* rootNode, char* operator, char* command, char* arguments);

/**
 * Print all the node tree
 * 
 * \param struct Node* root node to display
 * \return void
 */
void printNodes(struct Node* node);

/**
 * Trim leading and trailing whitespaces of the char*
 * 
 * \param char* str that need whitespaces to be removed
 * \return char* str without leading and trailing whitespaces
 */
char* trimWhitepaces(char* str);