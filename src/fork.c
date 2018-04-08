/**
 * \file fork.c
 * \brief contain all fork related functions
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date February 27, 2018 
 * 
 * Contain all function that allows background et foreground
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

#include "../include/typedef.h"
#include "../include/define.h"
#include "../include/utils.h"
#include "../include/fork.h"

extern ForkMap **forkMapRunning;

void initForkMap(ForkMap **forkMap) {
  int i;

  for(i = 0; i < FORKMAP_SIZE; i++) {
    if(forkMap[i] != NULL) {
      forkMap[i] = NULL;
    }
  }
}

void FillForkMap(ForkMap **forkMap, int pid, char *value) {
  int i;
  
  for(i = 0; i < FORKMAP_SIZE; i++) {
    if(forkMap[i] == NULL) {
      printf("[%d]          %d : %s\n", i + 1, pid, value);
      forkMap[i] = malloc(sizeof(ForkMap));
      forkMap[i]->index = i + 1;
      forkMap[i]->pid = pid;
      forkMap[i]->value = malloc(sizeof(char*));
      strcpy(forkMap[i]->value, value);
      break;
    }
  }
}

void ManageForkMap(ForkMap **forkMap1, ForkMap **forkMap2) {
  pid_t pid;
  int i = 0;
  int j = 0;  
  
  while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    while(forkMap1[i] != NULL) 
      i++;
    for(j = 0; j < FORKMAP_SIZE; j++) {
      if(forkMap2[j] != NULL && forkMap2[j]->pid == pid) 
        break;
    }
    if(forkMap2[j] != NULL) {
      forkMap1[i] = malloc(sizeof(ForkMap));
      forkMap1[i]->index = forkMap2[j]->index;
      forkMap1[i]->pid = forkMap2[j]->pid;
      forkMap1[i]->value = malloc(sizeof(char*));
      strcpy(forkMap1[i]->value, forkMap2[j]->value);
      freeForkMap(forkMap2, j);
    }
  }
}

void printForkMap(ForkMap **forkMap) {
  int i;
  
  for(i = 0; i < FORKMAP_SIZE; i++) {
    if(forkMap[i] != NULL) {
      printf("[%d]+ Done    %d : %s\n", forkMap[i]->index, forkMap[i]->pid, forkMap[i]->value);
      freeForkMap(forkMap, i);
    }
  }
}

void freeForkMap(ForkMap **forkMap, int index) {
  freeIfNeeded(forkMap[index]->value);
  freeIfNeeded(forkMap[index]);
  forkMap[index] = NULL;
}

void makeForeground(int index) {
  if(forkMapRunning[index] == NULL) 
    return;

  printf("%s\n", forkMapRunning[index]->value);
  waitpid(forkMapRunning[index]->pid, NULL, 0);
  freeForkMap(forkMapRunning, index);
}