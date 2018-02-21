/**
 * \file handler.c
 * \brief handle the commands in the tree
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date February 6, 2018 
 * 
 * Execute all the command in the node tree 
 */

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include "../include/typedef.h"
#include "../include/check.h"
#include "../include/utils.h"
#include "../include/executer.h"
#include "../include/handler.h"

#define BUFFERSIZE 4096 //TODO A FACTO

void handle(Node *rootNode) {
  Node *node = rootNode;
  bool run = true;

  if(rootNode == NULL || (memcmp(rootNode, node, sizeof(Node)) == 0 && strcmp("", node->action->command) == 0)){
    return;
  }
  
  if(node->next == NULL) {
      execute(node, false);
  } else {
    while(node != NULL && run) {
      if(strcmp("&&", node->operator) == 0) {
        run = execute(node, false);
      } else if (strcmp("||", node->operator) == 0) {
        run = !execute(node, false);
      } else if (strcmp("|", node->operator) == 0) {
        handlePipe(node, node->next);
        node = node->next; // Skip a node
      } else {
        execute(node, false);
      }

      node = node->next;
    }
  }
  
  freeIfNeeded(rootNode);
}

void handlePipe(Node *nodeInput, Node *nodeOutput) {
  pid_t pidInput, pidOutput; 

  int fileDescriptor[2]; 
  int status = 0;

  if(nodeOutput != NULL) {
    CHECK(pipe(fileDescriptor) == 0);

    if((pidInput = fork()) == 0) {
      close(fileDescriptor[0]);
      dup2(fileDescriptor[1], 1);

      execute(nodeInput, true);
    } else if(pidInput == -1) {
      perror("Input fork failed\n");
      exit(EXIT_FAILURE);
    }

    if((pidOutput = fork()) == 0) {
      close(fileDescriptor[1]);
      dup2(fileDescriptor[0], 0);

      execute(nodeOutput, true);
    } else if(pidOutput == -1) {
      perror("Output fork failed\n");
      exit(EXIT_FAILURE);
    }

    close(fileDescriptor[1]);      
    close(fileDescriptor[0]);
    waitpid(pidInput, &status, 0);
    waitpid(pidOutput, &status, 0);
  }
}