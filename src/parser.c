/**
 * \file parser.c
 * \brief parse char into a tree.
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date January 26, 2018 
 * parse the command in char in a tree and return the first node
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/typedef.h"
#include "../include/parser.h"
#include "../include/define.h"

LinkedList *parse(char *commandParam) {
  bool isBackgrounded = false;
  bool isParsingArguments = false;
  char commandParamCopy[BUFFER_SIZE] = "";
  char operator[BUFFER_SIZE] = "";
  char command[BUFFER_SIZE] = "";
  char arguments[BUFFER_SIZE] = "";
  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;
  Node *rootNode = NULL;
  Node *node = NULL;

  strcpy(commandParamCopy, commandParam);

  while(isspace((unsigned char)commandParamCopy[i]))
      i++; // Trim leading whitespace
  
  while(commandParamCopy[i] != '\0') {
    // Search for operators
    if(strchr("|",commandParamCopy[i]) != NULL || strchr("&",commandParamCopy[i]) != NULL || strchr(">",commandParamCopy[i]) != NULL || strchr("<",commandParamCopy[i]) != NULL) {
      if(strlen(commandParamCopy) > i && commandParamCopy[i+1] == '\0') {
        isBackgrounded = true;
      } else {
        while (strchr("|",commandParamCopy[i]) != NULL || strchr("&",commandParamCopy[i])!= NULL || strchr(">",commandParamCopy[i]) != NULL || strchr("<",commandParamCopy[i]) != NULL) {
          operator[j] = commandParamCopy[i];
          i++;
          j++;
        }

        while(isspace((unsigned char)commandParamCopy[i]))
          i++;
        
        if(rootNode == NULL) {
          rootNode = createNode(operator, command, arguments);
          node = rootNode;
        } else {
          node = addNode(node, operator, command, arguments);
        }

        memset(operator, 0, sizeof(operator));
        memset(command, 0, sizeof(command));
        memset(arguments, 0, sizeof(arguments));
        
        j = 0;
        k = 0;
        l = 0;

        isParsingArguments = false;
      }
    }
    if(isspace((unsigned char)commandParamCopy[i]) && !isParsingArguments ) {
      isParsingArguments = true;
    } 
    if(!isBackgrounded) {
      if(!isParsingArguments){
        command[k] = commandParamCopy[i];
        k++;
      } else {
        arguments[l] = commandParamCopy[i];
        l++;
      }
    }
    i++;
  }
  // When the while is over, we add the last command
  if(command != NULL){
    if(rootNode == NULL) {
      rootNode = createNode(operator, command, arguments);
    } else {
      node = addNode(node, operator, command, arguments);
    }
  }

  //printNodes(rootNode);
  return createLinkedList(rootNode, isBackgrounded);
}

LinkedList *createLinkedList(Node *rootNode, bool isBackgrounded) {
  LinkedList *linkedList = malloc(sizeof(LinkedList));

  linkedList->rootNode = rootNode;
  linkedList->isBackgrounded = isBackgrounded;

return linkedList;
}

Node *createNode(char *operator, char *command, char *arguments) {
  Node* node = malloc(sizeof(Node));
  Action* action = malloc(sizeof(Action));
  node->operator = malloc(BUFFER_SIZE * sizeof(char));
  action->command = malloc(BUFFER_SIZE * sizeof(char));
  action->arguments = malloc(BUFFER_SIZE * sizeof(char));

  strcpy(node->operator, trimWhitepaces(operator));
  strcpy(action->command, trimWhitepaces(command));
  strcpy(action->arguments, trimWhitepaces(arguments));

  node->action = action;
  node->next = NULL;

  return node;
}

Node *addNode(Node *node, char *operator, char *command, char *arguments) {
  Node* next;
  
  if(node != NULL) {
    next = createNode(operator, command, arguments);
    node->next = next;
    node = next;
  }

  return node;
}

void printNodes(Node *node) {
  printf("Nodes :\n");
  while(node != NULL) {
    printf("  operator : %s, command : %s, arguments : %s\n", node->operator, node->action->command, node->action->arguments);
    node = node->next;
  }
}

char *trimWhitepaces(char *str) {
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;
  if(*str == 0)
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) 
    end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}