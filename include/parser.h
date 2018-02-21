#ifndef __PARSER_H__
#define __PARSER_H__

/**
 * Parse the commmand in parameter
 * 
 * \param char* commandParam to parse
 * \return Tree *tree return the command into a Tree struct
 */
Tree *parse(char *commandParam);

/**
 * Create and return a new Tree fromm rootNode and isBackground
 * 
 * \param Node *rootNode
 * \param bool isBackground
 * \return Tree *tree
 */
Tree *createTree(Node *rootNode, bool isBackground);

/**
 * Create and return a new Node from operator and command in parameters 
 * 
 * \param char *operator to add in the node
 * \param char *command to add in the node
 * \param char *arguments to add in the node
 * \return Node *newly created
 */
Node *createNode(char *operator, char *command, char *arguments);

/**
 * Add the next node to a node and return the next node
 * 
 * \param Node *node to have a next one
 * \param char *operator to add in the next node
 * \param char *command to add in the node
 * \param char *arguments to add in the node
 * \return Node *node created
 */
Node *addNode(Node *node, char *operator, char *command, char *arguments);

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