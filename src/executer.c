/**
 * \file execute.c
 * \brief execute the command in the tree
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date January 26, 2018 
 * 
 * Execute all the command in the node tree 
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/typedef.h"
#include "../include/utils.h"
#include "../include/check.h"
#include "../include/manageEnvVar.h"
#include "../include/executer.h"

extern bool run;

void execute(struct Node* node) {
  if(node != NULL) {
    if(!checkBuildInCommand(node)) {
      executeCommand(node);
    }
  }
}

bool checkBuildInCommand(struct Node* node) {
  bool isBuildInCommand = true;

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
  } else 
      isBuildInCommand = false;

  return isBuildInCommand;
}

void executeCommand(struct Node* node) {
  pid_t pid;
  int status = 0;
  char** action  = NULL;
  
  CHECK(node->action != NULL);
  
  pid = fork();
  CHECK(pid != -1);
  
  if (pid == 0) {
    fillActionArray(&action, node->action->command, node->action->arguments);
    execvp(action[0], action);

    printf("%s : unknow command\n", node->action->command);
  } else {
    wait(&status);
    free(action);
  }
}

void changeDirectory(char* path) {
  if(strcmp(path, "") == 0) {
    CHECK(chdir(getEnvVar("HOME")) != -1);
  } else {
    CHECK(chdir(path) != -1);
  }
}

void printWorkingDirectory(){
  printf("%s\n", getWorkingDirectory());
}

void echo(char* str) {
  str = str_replace(str, "\"", "");
  printf("%s\n", str);
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
  int size = 0;
  char*  argumentsStr = strtok(arguments, " ");
  char** tmp = NULL;
  
  // realloc for the command
  tmp = realloc(tmp, size++ * sizeof(char*));
  tmp[size-1] = command;
  // split string and append tokens to action
  while (argumentsStr) {
    tmp = realloc(tmp, size++ * sizeof(char*));
    tmp[size-1] = argumentsStr;
    argumentsStr = strtok(NULL, " ");
  }
  // realloc for the NULL
  tmp = realloc(tmp, size++ * sizeof(char*));
  tmp[size-1] = 0;

  *action = realloc(*action, size * sizeof(char*));
  memcpy(*action, tmp, size * sizeof(char*));
}