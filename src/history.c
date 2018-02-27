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
#include "../include/check.h"
#include "../include/reg.h"
#include "../include/check.h"
#include "../include/history.h"
#include "../include/define.h"

int historyResearch;

void manageHistory(Node *node) {
  if(strcmp(node->action->arguments, "") == 0)
    printHistory("", -1, NULL);
  else {

    const char *searchRegex = "(^(-s|--search)\\s[[:alnum:]]+)";

    char *historyCommand = NULL;

    if(checkRegex(searchRegex, &historyCommand, node->action->arguments)) {
      //Remove "-s " or "--search "
      while(*historyCommand != ' '){
        historyCommand++;
      }
      historyCommand++;
      printHistory(historyCommand, -1, NULL);
    }
  }
}

void printHistory(char *search, int commandNumber, char **command) {
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
          strcpy((*command),extractHistoryCommand(line));
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

char* extractHistoryCommand(char *command) {
  while(*command != '\t'){ // Remove number
    command++;
  }
  command++; // remove tabulation
  command[strlen(command) -1] = '\0'; // Replace \n with end string
  return command;
}

void readHistory(int way, char **str, int commandNumber) {
  if(way == 0 && historyResearch > 0)
    historyResearch--;
  else if(way == 1 && historyResearch < commandNumber)
    historyResearch++;

  printHistory("", historyResearch, str);  
}

void resetHistoryCounter(int commandNumber) {
  historyResearch = commandNumber;
}

int retrieveCommandNumber() {
  FILE *fpHistory;
  CHECK((fpHistory = fopen("/tmp/shellterHistory", "a+")) != NULL);
  fseek(fpHistory, 0, SEEK_END);

  if(ftell(fpHistory) > 0)
      return getCmdNum(fpHistory);

  fclose(fpHistory);

  return 1;
}

int getCmdNum(FILE *fpHistory) {

  int c;
  int i = -2;
  fseek(fpHistory, i, SEEK_END);
  while((c = fgetc(fpHistory)) != '\n') {
    i--;
    fseek(fpHistory, i, SEEK_END);
  }

  char *nbStr = malloc(BUFFER_SIZE * sizeof(char));
  memset(nbStr, 0, BUFFER_SIZE * sizeof(char));
  int l = 0;
  while((c = fgetc(fpHistory)) <= '9' && c >= '0' && c != EOF) {
    i++;
    fseek(fpHistory, i, SEEK_SET);
    nbStr[l] = (char)c;
    l++;
  }

  nbStr[l] = '\0';
  
  int number;
  if (sscanf(nbStr, "%d", &number) == 1) {
    return number + 1;
  }

  return 1;
}
