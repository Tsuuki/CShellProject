/**
 * Parse the commmand in parameter
 * \param char* command command to parse
 * \return struct node* return the command with the root node
 */
struct Node* parse(char*);

/**
 * Create and return a new node from operator and command in parameters 
 * \param char* operator operator to add in the node
 * \param char* command command to add in the node
 * \return struct Node*  node created
 */
struct Node* createNode(char*, char*);

/**
 * Add a nextNode to a node and return the nextNode
 * \param Node* node node to have a next one
 * \param char* operator operator to add in the next node
 * \return struct Node* node created
 */
struct Node* addNode(struct Node*, char*, char*);

/**
 * Print all the node tree
 *\param struct Node* root node to display
 */
void printNodes(struct Node* node);

/**
 * Trim leading and trailing whitespaces of the char*
 * \param char* str char* that need whitespaces to be removed
 * \return char* str without leading and trailing whitespaces
 */
char* trimWhitepaces(char*);