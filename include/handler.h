#ifndef __HANDLER_H__
#define __HANDLER_H__

/**
 * Handle the rootNode and call executer to execute the commands
 * 
 * \param strcut Node* rootNode of the tree
 * \return void
 */
void handle(Node *rootNode);

/**
 * Handle command using < and <<
 * 
 * \param Node *node command
 * \param char *file path to create file
 * \param char *mode to open file
 */
void handleRightRedirection(Node *node, char *file, char *mode, int descripteur);


/**
 * Handle command using a pipe
 * 
 * \param Node *nodeInput node in input of the pipe
 * \return int number of pipe handled
 */
int handlePipe(Node *node);

/**
 * Handle the pipe array recursively
 * 
 * \param Node *nodeArray[] array with all the nodes to be piped together
 * \param in inputFileDescriptor file descriptor
 * \param index index in the array
 * \return void
 */
void handlePipeArray(Node *nodeArray[], int inputFileDescriptor, int index);

/**
 * Handle command using <<
 * 
 * \param Node *node command
 * \param char *endWord get text untils end word is type
 * \return void
 */
void handleProgressivReading(Node *node, char *endWord);

/*
 * Calculate the number of pipe to handle
 * 
 * \param node with a pipe sign
 * \return int number of pipe to handle
 */
int getPipeNumber(Node *node);

#endif /** __HANDLER_H__ **/