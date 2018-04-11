#ifndef __PARSER_H__
#define __PARSER_H__

/**
 * Parse the commmand in parameter
 * 
 * \param char* commandParam to parse
 * \return Tree* return the command into a Tree struct
 */
LinkedList *parse(char *commandParam);

/**
 * Create and return a new LinkedList fromm rootNode and isBackground
 * 
 * \param Node *rootNode
 * \param bool isBackground
 * \return LinkedList*
 */
LinkedList *createLinkedList(Node *rootNode, bool isBackground);

/**
 * Create and return a new Node from operator and command in parameters 
 * 
 * \param char *operator to add in the node
 * \param char *command to add in the node
 * \param char *arguments to add in the node
 * \param Redirection *input to add in the node
 * \param Redirection *output to add in the node
 * \param Redirection *error to add in the node
 * \return Node *newly created
 */
Node *createNode(char *operator, char *command, char *arguments, Redirection *input, Redirection *output, Redirection *error);

/**
 * Create and return a new Redirection from type and file in parameters 
 * 
 * \param char *type to add in the Redirection
 * \param char *file to add in the Redirection
 * \return Redirection *newly created
 */
Redirection *createRedirection(char *type, char *file);

/**
 * Add the next node to a node and return the next node
 * 
 * \param Node *node to have a next one
 * \param char *operator to add in the next node
 * \param char *command to add in the node
 * \param char *arguments to add in the node
 * \param Redirection *input to add in the node
 * \param Redirection *output to add in the node
 * \param Redirection *error to add in the node
 * \return Node *node created
 */
Node *addNode(Node *node, char *operator, char *command, char *arguments, Redirection *input, Redirection *output, Redirection *error);

/**
 * Print all the node tree
 * 
 * \param Node *root node to display
 * \return void
 */
void printNodes(Node *node);

/**
 * Trim leading and trailing whitespaces of the char*
 * 
 * \param char *str that need whitespaces to be removed
 * \return char *str without leading and trailing whitespaces
 */
char *trimWhitepaces(char *str);

#endif /** __PARSER_H__ **/