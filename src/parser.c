/**
 * \file parser.c
 * \brief parse char into a linkedList.
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date January 26, 2018 
 * parse the command in char in a linkedList and return the linkedList
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
  char redirection[BUFFER_SIZE] = "";
  char redirectionFile[BUFFER_SIZE] = "";
  char command[BUFFER_SIZE] = "";
  char arguments[BUFFER_SIZE] = "";
  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;
  int m = 0;
  Node *rootNode = NULL;
  Node *node = NULL;

  Redirection *input = NULL;
  Redirection *output = NULL;

  /*rootNode = createNode("|", "sort", "-n", input, NULL, NULL);
  node = createNode("", "head" , "-3", NULL, output, NULL);
  rootNode->next = node;*/

  strcpy(commandParamCopy, commandParam);

  while(isspace((unsigned char)commandParamCopy[i]))
      i++; // Trim leading whitespace
  
  while(commandParamCopy[i] != '\0') {
    // Search for operators
    if(strchr("|",commandParamCopy[i]) != NULL || strchr("&",commandParamCopy[i]) != NULL || strchr(";",commandParamCopy[i]) != NULL) {
      if(strlen(commandParamCopy) > i && commandParamCopy[i+1] == '\0') {
        isBackgrounded = true;
      } else {
        while (strchr("|",commandParamCopy[i]) != NULL || strchr("&",commandParamCopy[i])!= NULL || strchr(";",commandParamCopy[i])!= NULL) {
          operator[j] = commandParamCopy[i];
          i++;
          j++;
        }

        while(isspace((unsigned char)commandParamCopy[i]))
          i++;
        

        if(rootNode == NULL) {
          rootNode = createNode(operator, command, arguments, input, output, NULL);
          node = rootNode;
        } else {
          node = addNode(node, operator, command, arguments, input, output, NULL);
        }

        input = NULL;
        output = NULL;

        memset(operator, 0, sizeof(operator));
        memset(command, 0, sizeof(command));
        memset(arguments, 0, sizeof(arguments));
        memset(redirection, 0, sizeof(redirection));
        memset(redirectionFile, 0, sizeof(redirectionFile));
        
        j = 0;
        k = 0;
        l = 0;

        isParsingArguments = false;
      }
    } // Search for redirection
    if(strchr(">",commandParamCopy[i]) != NULL || strchr("<",commandParamCopy[i]) != NULL) {
      while (strchr(">",commandParamCopy[i]) != NULL || strchr("<",commandParamCopy[i]) != NULL) {
        redirection[j] = commandParamCopy[i];
        i++;
        j++;
      }

     while(isspace((unsigned char)commandParamCopy[i]))
        i++; // Trim space between redirection and file

      while(commandParamCopy[i] != '\0' && commandParamCopy[i] != ' ') {
        redirectionFile[m] = commandParamCopy[i];
        i++;
        m++;
      }

      if(strcmp("<", redirection) == 0 || strcmp("<<", redirection) == 0) {
        input = createRedirection(redirection, redirectionFile);
      }

      if(strcmp(">", redirection) == 0 || strcmp(">>", redirection) == 0) {
        output = createRedirection(redirection, redirectionFile);
      }

      j = m = 0;
    }
    if(isspace((unsigned char)commandParamCopy[i]) && !isParsingArguments && strlen(command) > 0) {
      isParsingArguments = true;
    } 
    if(!isBackgrounded) {
      if(!isParsingArguments){
        while(isspace((unsigned char)commandParamCopy[i]))
          i++;
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
      rootNode = createNode(operator, command, arguments, input, output, NULL);
    } else {
      node = addNode(node, operator, command, arguments, input, output, NULL);
    }
    input = NULL;
    output = NULL;
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

Redirection *createRedirection(char *type, char *file) {
  Redirection *redirection = malloc(sizeof(Redirection));
  redirection->type = malloc(SMALL_BUFFER_SIZE * sizeof(char));
  redirection->file = malloc(BUFFER_SIZE * sizeof(char));

  strcpy(redirection->type, type);
  strcpy(redirection->file, file);

  return redirection;
}

Node *createNode(char *operator, char *command, char *arguments, Redirection *input, Redirection *output, Redirection *error) {
  Node *node = malloc(sizeof(Node));
  Action *action = malloc(sizeof(Action));
  node->operator = malloc(BUFFER_SIZE * sizeof(char));
  action->command = malloc(BUFFER_SIZE * sizeof(char));
  action->arguments = malloc(BUFFER_SIZE * sizeof(char));
  node->input = NULL;
  node->output = NULL;
  node->error = NULL;

  if(input != NULL)
    node->input = input;

  if(output != NULL) 
    node->output = output;

  if(error != NULL) 
    node->error = error;

  strcpy(node->operator, operator);
  strcpy(action->command, command);
  strcpy(action->arguments, arguments);

  node->action = action;
  node->next = NULL;

  return node;
}

Node *addNode(Node *node, char *operator, char *command, char *arguments, Redirection *input, Redirection *output, Redirection *error) {
  Node* next;
  
  if(node != NULL) {
    next = createNode(operator, command, arguments, input, output, error);
    node->next = next;
    node = next;
  }

  return node;
}

void printNodes(Node *node) {
  printf("Nodes :\n");
  while(node != NULL) {
    printf("  operator : %s, command : %s, arguments : %s\n", node->operator, node->action->command, node->action->arguments);
    if(node->input != NULL)
      printf("%8s %s %8s %s\n","input", node->input->type, "file", node->input->file);

    if(node->output != NULL)
      printf("%8s %s %8s %s\n","output", node->output->type, "file", node->output->file);

    if(node->error != NULL)
      printf("%8s %s %8s %s\n","error", node->error->type, "file", node->error->file);

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