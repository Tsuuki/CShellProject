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
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/typedef.h"
#include "../include/check.h"
#include "../include/utils.h"
#include "../include/manageEnvVar.h"
#include "../include/executer.h"

#define MAX_ARGUMENTS 20

extern bool run;

bool execute(Node *node) {
  bool isExecuted = true;
  if(node != NULL) {
    if(!checkBuildInCommand(node)) {
      switch(executeCommand(node)) {
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
    }
  }
  return isExecuted;
}

bool checkBuildInCommand(Node *node) {
  bool isExecuted = true;

  if(strcmp(node->action->command, "cd") == 0) {
    changeDirectory(node->action->arguments);
  } else if(strcmp(node->action->command, "pwd") == 0) {
    printWorkingDirectory();
  } else if(strcmp(node->action->command, "echo") == 0) {
    echo(node->action->arguments);
  } else if(strcmp(node->action->command, "printvar") == 0) {
    printEnvVar();
  } else if(strcmp(node->action->command, "addvar") == 0) {
    addEnvVar(node->action->arguments);
  } else if(strcmp(node->action->command, "delvar") == 0) {
    delEnvVar(node->action->arguments);
  } else if(strcmp(node->action->command, "history") == 0) {
    printHistory();
  } else if(strcmp(node->action->command, "exit") == 0) {
    exitShell();
  } else {
    isExecuted = false;
  }

  return isExecuted;
}

int executeCommand(Node *node) {
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
  if(strcmp(str, "") != 0) {
    if(strchr("$", str[0])) {
        str++;
        char *value = getEnvVar(str);
        if(value != NULL) {
          printf("%s\n", getEnvVar(str));
        } else {
          printf("%s : unkonw environment variable\n", str);
        }
    } else {
      printf("%s\n", str);
    }
  }  
}

void exitShell() { 
  run = false;
}

void printHistory() {
  int c;
  FILE *file;
  CHECK((file = fopen("/tmp/shellterHistory", "r")) != NULL);
  if(file) {
    while((c = getc(file)) != EOF) {
      putchar(c);
    }
    fclose(file);
  }
}

void fillActionArray(char*** action, char* command, char* arguments) {
  int i = 0;
  char *argumentsStr = strtok(arguments, " ");
  char *tmp[MAX_ARGUMENTS];

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