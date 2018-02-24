/**
 * \file history.c
 * \brief All functions on alias
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date February 22, 2018 
 * 
 * File containing all the history functions
 */
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../include/typedef.h"
#include "../include/history.h"
#include "../include/check.h"
#include "../include/reg.h"
#include "../include/check.h"

int historyResearch;

void manageHistory(Node *node) {
  if(strcmp(node->action->arguments, "") == 0)
    printHistory("", -1);
  else {

    const char *searchRegex = "(^(-s|--search)\\s[[:alnum:]]+)";

    char *historyCommand = NULL;

    if(checkRegex(searchRegex, &historyCommand, node->action->arguments)) {
      //Remove "-s " or "--search "
      while(*historyCommand != ' '){
        historyCommand++;
      }
      historyCommand++;
      printHistory(historyCommand, -1);
    }
  }
}

void printHistory(char *search, int commandNumber) {
  FILE *file;
  CHECK((file = fopen("/tmp/shellterHistory", "r")) != NULL);

  if(file) {
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;

    while ((read = getline(&line, &len, file)) != -1) {
      if(strlen(search) > 0) {
        if(strstr(line, search)) {
          printf("%s", line);
        }
      } else if(commandNumber != -1) {
        if(i == commandNumber) {
          printHistoryCommand(line);
          break;
        }
      } else {
        printf("%s", line);
      }
      i++;
    }
    fclose(file);
  }
}

void printHistoryCommand(char *command) {
  while(*command != '\t'){
    command++;
  }
  command++;
  printf("%s", command);
}

void readHistory(int way, char **str) {
  if(way == 0)
    historyResearch--;
  else if(way == 1)
    historyResearch++;

  printHistory("", historyResearch);  
}

void resetHistoryCounter(int commandNumber) {
  historyResearch = commandNumber;
}
