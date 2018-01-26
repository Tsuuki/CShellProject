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

#include "../include/parser.h"
#include "../include/typedef.h"

#define BUFFERSIZE 4096

struct Node* parse(char* commandParam) {
  int i, j, k = 0;
  char commandParamCopy[BUFFERSIZE] = "";
  char operator[BUFFERSIZE] = "";
  char command[BUFFERSIZE] = "";
  Node* rootNode = NULL;
  Node* node = NULL;

  strcpy(commandParamCopy, commandParam);
  while(commandParamCopy[i] != '\0') {
    if(strchr("|",commandParamCopy[i]) != NULL || strchr("&",commandParamCopy[i]) != NULL || strchr(">",commandParamCopy[i]) != NULL || strchr("<",commandParamCopy[i]) != NULL) {
      while (strchr("|",commandParamCopy[i]) != NULL || strchr("&",commandParamCopy[i])!= NULL || strchr(">",commandParamCopy[i]) != NULL || strchr("<",commandParamCopy[i]) != NULL) {
        operator[k] = commandParamCopy[i];
        i++;
        k++;
      }
      if(rootNode == NULL) {
        rootNode = createNode(operator, command);
      } else if(rootNode->nextNode == NULL) {
        node = addNode(rootNode, operator, command);
      } else {
        node = addNode(node, operator, command);
      }

      strcpy(operator, "");
      strcpy(command, "");
      k = 0;
      j = 0;
    }
    command[j] = commandParamCopy[i];
    i++;
    j++;
  }

  if(command != NULL) {
    if(rootNode == NULL) { // A FACTORISER !
      rootNode = createNode(operator, command);
    } else if(rootNode->nextNode == NULL) {
      node = addNode(rootNode, operator, command);
    } else {
      node = addNode(node, operator, command);
    }
  }

  return rootNode;
}

void printNodes(struct Node* node) {
  printf("Nodes :\n");
  while(node != NULL) {
    printf("  operator : %s, command : %s\n", node->operator, node->command);
    node = node->nextNode;
  }
}

struct Node* createNode(char* operator, char* command) {
  Node* node = malloc(sizeof(node));
  node->operator = malloc(BUFFERSIZE*sizeof(char));
  node->command = malloc(BUFFERSIZE*sizeof(char));

  strcpy(node->operator, trimWhitepaces(operator));
  strcpy(node->command, trimWhitepaces(command));
  node->nextNode = NULL;

  return node;
}

struct Node* addNode(struct Node* node, char* operator, char* command) {
  Node* nextNode;

  if(node != NULL) {
    nextNode = createNode(operator, command);
    node->nextNode = nextNode;
  }

  return nextNode;
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