/**
 *\file execute.c
 *\brief execute the command in the tree
 *\author Quentin D. Jordan H.
 *\version 1.0
 *\date January 26, 2018 
 *
 *Execute all the command in the node tree 
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "../include/typedef.h"
#include "../include/check.h"
#include "../include/utils.h"
#include "../include/environment.h"
#include "../include/executer.h"
#include "../include/alias.h"
#include "../include/parser.h"
#include "../include/history.h"
#include "../include/authors.h"
#include "../include/define.h"

extern bool run;
extern struct AliasArray *aliases;

bool execute(Node *node, bool isForked) {
  bool isExecuted = false;

  if(node != NULL) {
    if(!checkBuildInCommand(&node)) {
      if(isForked) {
        isExecuted = checkResult(node, executeCommandForked(node));
      } else {
        isExecuted = checkResult(node, executeCommand(node));
      }
    } else {
      isExecuted = true;
    }
  }

  return isExecuted;
}

bool checkBuildInCommand(Node **node) {
  bool isExecuted = true;
  int c;

  if((c = isAliasExist(node[0]->action->command)) != -1){
    node[0] = (parse((aliases->aliases[c]).command))->rootNode;
  }

  if(strcmp(node[0]->action->command, "cd") == 0) {
    changeDirectory(node[0]->action->arguments);
  } else if(strcmp(node[0]->action->command, "pwd") == 0) {
    printWorkingDirectory();
  } else if(strcmp(node[0]->action->command, "echo") == 0) {
    echo(node[0]->action->arguments);
  } else if(strcmp(node[0]->action->command, "printvar") == 0) {
    printEnvVar();
  } else if(strcmp(node[0]->action->command, "addvar") == 0) {
    addEnvVar(node[0]->action->arguments);
  } else if(strcmp(node[0]->action->command, "delvar") == 0) {
    delEnvVar(node[0]->action->arguments);
  } else if(strcmp(node[0]->action->command, "history") == 0) {
    manageHistory(node[0]);
  } else if(strcmp(node[0]->action->command, "alias") == 0) {
    manageAlias(node[0]);
  } else if(strcmp(node[0]->action->command, "authors") == 0) {
    printAuthors();
  } else if(strcmp(node[0]->action->command, "exit") == 0) {
    exitShell();
  } else {
    isExecuted = false;
  }

  return isExecuted;
}

bool checkResult(Node *node, int code) {
  bool isExecuted = true;

  switch(code) {
    case 1 :
      isExecuted = false;
      break;
    case 2 :
      isExecuted = false;
      printf("%s : unknow command\n", node->action->command);
      break;
    case 3 :
      isExecuted = false;
      printf("wrong command\n");
      break;
  }

  return isExecuted;
}

int executeCommand(Node *node) {
  char **action  = NULL;

  if(node->action == NULL) 
    return 3;

  fillActionArray(&action, node->action->command, node->action->arguments);
  execvp(action[0], action);
  exit(errno);
}

int executeCommandForked(Node *node) {
  pid_t pid;
  int code = true;
  int status = 0;
  char **action  = NULL;

  if(node->action == NULL) 
    return 3;

  pid = fork();
  CHECK(pid != -1);
  if (pid == 0) {
    fillActionArray(&action, node->action->command, node->action->arguments);
    execvp(action[0], action);

    exit(errno);
  } else {
    wait(&status);
    if(WIFEXITED(status))
      code = WEXITSTATUS(status);
    freeIfNeeded(action);
  }

  return code;
}

void changeDirectory(char *path) {
  if(strcmp(path, "") == 0) {
    CHECK(chdir(getEnvVar("HOME")) != -1);
  } else {
    CHECK(chdir(path) != -1);
  }
}

void printWorkingDirectory(){
  printf("%s\n", getWorkingDirectory());
}

void echo(char *str) {
  if(strlen(str) > 0) {

    char *env = malloc(sizeof(char) * 4096);
    char *value;
    int j = 0;

    for(int i = 0; i < strlen(str); i++) {
      if(str[i] == '$') {

        i+= 1;
        while(str[i] != '\0' && str[i] != ' ') {
          env[j] = str[i];
          i++;
          j++;
        }
        env[j] = '\0';

        value = getEnvVar(env);
        if(value != NULL) {
          printf("%s ", value);
        }
      }
      else {
        printf("%c", str[i]);
      }
    }
    printf("\n");
  }  
}

void exitShell() { 
  run = false;
}

void fillActionArray(char ***action, char *command, char *arguments) {
  int i = 0;
  char **tmp = malloc(getSize(arguments) * sizeof(char *));
  char *argumentsStr = strtok(arguments, " ");

  // realloc for the command
  tmp[i] = command;
  i++;

  // split string and append tokens to action
  while (argumentsStr) {
    tmp[i] = argumentsStr;
    i++;
    argumentsStr = strtok(NULL, " ");
  }
  i++;
  // realloc for the NULL
  tmp[i-1] = 0;

  *action = realloc(*action, i * sizeof(char *));
  memcpy(*action, tmp, i * sizeof(char*));
}

int getSize(char *arguments) {
  int i;
  int size = 0;

  for(i = 0; arguments[i] != '\0'; i++) {
    if(isspace(arguments[i])) {
      size++;
    }
  }

  return size + 3;
}