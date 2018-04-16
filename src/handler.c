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
#include <fcntl.h>
#include <string.h>

#include "../include/typedef.h"
#include "../include/check.h"
#include "../include/utils.h"
#include "../include/executer.h"
#include "../include/handler.h"

void handle(Node *rootNode) {
  bool run = true;
  int number;
  Node *node = rootNode;

  // If command is empty
  if(rootNode == NULL || (memcmp(rootNode, node, sizeof(Node)) == 0 && strcmp("", node->action->command) == 0)){
    return;
  }

  if(node->next == NULL) {
      execute(node, true);
  } else {
    while(node != NULL && run) {
      if(strcmp("&&", node->operator) == 0) {
        run = execute(node, true);
      } else if (strcmp("||", node->operator) == 0) {
        run = !execute(node, true);
      } else if (strcmp("|", node->operator) == 0) {
        number = handlePipe(node);
        while(number > 1) {
          node = node->next;
          number--;
        }
      } else {
        execute(node, true);
      }
      node = node->next;
    }
  }
  
  freeIfNeeded(rootNode);
}

int handlePipe(Node *node) {
  pid_t pid;
  int i;
  int number = getPipeNumber(node);
  int status = 0;
  Node *nodeArray[number+1];
  
  for(i = 0; i < number; i++) {
    nodeArray[i] = node;
    node = node->next;
  }
  nodeArray[number] = NULL;
  CHECK((pid = fork()) != -1);
  if(pid == 0) {
    handlePipeArray(nodeArray, STDIN_FILENO, 0);
  }
    
  waitpid(pid, &status, 0);

  return number;
}

void handlePipeArray(Node *nodeArray[], int inputFileDescriptor, int index) {
  pid_t pid;
  int fileDescriptor[2];

  if(nodeArray[index+1] == NULL) {
    dup2(inputFileDescriptor, STDIN_FILENO);
    execute(nodeArray[index], false);
  } else {
    CHECK(pipe(fileDescriptor) == 0);
    CHECK((pid = fork()) != -1);

    if(pid == 0) {
      close(fileDescriptor[0]);
      dup2(inputFileDescriptor, STDIN_FILENO);
      dup2(fileDescriptor[1],STDOUT_FILENO);
      execute(nodeArray[index], false);
    } else { 
      close(fileDescriptor[1]);      
      close(inputFileDescriptor);
      handlePipeArray(nodeArray, fileDescriptor[0], index+1);
    }
  }
  return;
}

int getPipeNumber(Node *node) {
  int number = 1;

  while(node->next != NULL && strcmp("|", node->operator) == 0) {
    node = node->next;
    number++;
  }

  return number;
}