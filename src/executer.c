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

#include "../include/typedef.h"
#include "../include/utils.h"
#include "../include/executer.h"
#include "../include/check.h"
#include "../include/manageEnvVar.h"

extern bool run;

void execute(struct Node* rootNode) {
  Node* node = rootNode;
  
  while(node != NULL) {
    checkBuildInCommand(node);
    node = node->next;
  }
}

void checkBuildInCommand(struct Node* node) {
  if(strcmp(node->action->command, "cd") == 0) {
    changeDirectory(node->action->arguments);
  }
  if(strcmp(node->action->command, "pwd") == 0) {
    printWorkingDirectory();
  }
  if(strcmp(node->action->command, "echo") == 0) {
    echo(node->action->arguments);
  }
  if(strcmp(node->action->command, "exit") == 0) {
    exitShell();
  }
}

void changeDirectory(char* path) {
  if(path == NULL) {
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