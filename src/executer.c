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
#include "../include/executer.h"
#include "../include/check.h"
#include "../include/manageEnvVar.h"

extern bool run;

void execute(struct Node* rootNode) {
  Node* node = rootNode;
  
  while(node != NULL) {
    if(!checkBuildInCommand(node)) {
      executeCommand(node);
    }
    node = node->next;
  }
}

bool checkBuildInCommand(struct Node* node) {
  bool isBuildInCommand = false;

  if(strcmp(node->action->command, "cd") == 0) {
    changeDirectory(node->action->arguments);
    isBuildInCommand = true;
  } else if(strcmp(node->action->command, "pwd") == 0) {
    printWorkingDirectory();
    isBuildInCommand = true;
  } else if(strcmp(node->action->command, "echo") == 0) {
    echo(node->action->arguments);
    isBuildInCommand = true;
  } else if(strcmp(node->action->command, "exit") == 0) {
    exitShell();
    isBuildInCommand = true;
  }

  return isBuildInCommand;
}

void executeCommand(struct Node* node) {
  pid_t pid;
  int status = 0;
  
  CHECK(node->action != NULL);
  
  pid = fork();
  CHECK(pid != -1);
  
  if (pid == 0) {
    int i = 0;
    char*  arguments = strtok (node->action->arguments, " ");
    char** action  = NULL;

    // realloc for the command
    action = realloc (action, ++i * sizeof(char*));
    action[i-1] = node->action->command;
    // split string and append tokens to action
    while (arguments) {
      action = realloc (action, ++i * sizeof(char*));
      action[i-1] = arguments;
      arguments = strtok (NULL, " ");
    }
    // realloc for the NULL
    action = realloc (action, (i+1) * sizeof(char*));
    action[i] = 0;

    execvp(action[0], action);
  } else {
    wait(&status);
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