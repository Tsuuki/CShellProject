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

#define BUFFERSIZE 4096

struct Node* parse(char* commandParam) {
  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;
  bool parsingArguments = false;
  char commandParamCopy[BUFFERSIZE] = "";
  char operator[BUFFERSIZE] = "";
  char command[BUFFERSIZE] = "";
  char arguments[BUFFERSIZE] = "";
  Node* rootNode = NULL;
  Node* node = NULL;

  strcpy(commandParamCopy, commandParam);

  while(commandParamCopy[i] != '\0') {
    // Search for operators
    if(strchr("|",commandParamCopy[i]) != NULL || strchr("&",commandParamCopy[i]) != NULL || strchr(">",commandParamCopy[i]) != NULL || strchr("<",commandParamCopy[i]) != NULL) {
      while (strchr("|",commandParamCopy[i]) != NULL || strchr("&",commandParamCopy[i])!= NULL || strchr(">",commandParamCopy[i]) != NULL || strchr("<",commandParamCopy[i]) != NULL) {
        operator[j] = commandParamCopy[i];
        i++;
        j++;
      }
      
      if(rootNode == NULL) {
        rootNode = createNode(operator, command, arguments);
        node = rootNode;
      } else {
        node = addNode(node, operator, command, arguments);
      }

      memset(operator, 0, sizeof(operator));
      memset(command, 0, sizeof(command));
      memset(command, 0, sizeof(arguments));
      j = 0;
      k = 0;
      l = 0;
      parsingArguments = false;
    }
    if(isspace((unsigned char)commandParamCopy[i]) && parsingArguments != true) {
      parsingArguments = true;
      i++; // Skip the whitespace
    } 
    if(!parsingArguments){
      command[k] = commandParamCopy[i];
      k++;
    } else {
      arguments[l] = commandParamCopy[i];
      l++;
    }
    i++;
  }
  // When the while is over, we add the last command
  if(command != NULL){
    if(rootNode == NULL) {
      rootNode = createNode(operator, command, arguments);
      node = rootNode;
    } else {
      node = addNode(node, operator, command, arguments);
    }
  }

  return rootNode;
}

struct Node* createNode(char* operator, char* command, char* arguments) {
  Node* node = malloc(sizeof(node));
  Action* action = malloc(sizeof(Action));
  node->operator = malloc(BUFFERSIZE * sizeof(char));
  action->command = malloc(BUFFERSIZE * sizeof(char));
  action->arguments = malloc(BUFFERSIZE * sizeof(char));

  strcpy(node->operator, trimWhitepaces(operator));
  strcpy(action->command, trimWhitepaces(command));
  strcpy(action->arguments, trimWhitepaces(arguments));

  node->action = action;
  node->next = NULL;

  return node;
}

struct Node* addNode(struct Node* node, char* operator, char* command, char* arguments) {
  Node* next;

  if(node != NULL) {
    next = createNode(operator, command, arguments);
    node->next = next;
    node = next;
  }

  return node;
}

void printNodes(struct Node* node) {
  printf("Nodes :\n");
  while(node != NULL) {
    printf("  operator : %s, command : %s, arguments : %s\n", node->operator, node->action->command, node->action->arguments);
    node = node->next;
  }
}

char* trimWhitepaces(char* str) {
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;
  if(*str == 0)
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}