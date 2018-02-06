/**
 * \file manageEnvVar.c
 * \brief All functions on environment variables
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date January 25, 2018 
 * 
 * File containing all the functions on environment variables
 */

#include <unistd.h>
#include <stdio.h>

#include "../include/utils.h"
#include "../include/check.h"
#include "../include/manageEnvVar.h"

extern char** environ;

void printEnvVar() {
  int i = 0;
  
  while(environ[i]) {
      printf("%s\n", environ[i++]); 
  }
}

void addEnvVar(char* str) {
  char* string[2];
  string[0] = strtok(str, "=");
  string[1] = strtok(NULL, "=");
  
  if(string[0] == NULL || string[1] == NULL) {
    printf("incorrect arguments\n");
  } else {
    if(getenv(string[0]) != NULL) {
      printf("%s overwrittened\n", string[0]);
    } else {
      printf("%s created\n", string[0]);
    }
    
    CHECK(setenv(string[0], string[1], 1) != -1);
    freeIfNeeded(string);
  }
}

char* getEnvVar(char* name) {
  char* value = "";

  if(name == NULL) {
    printf("name is empty\n");
  } else {
    value = getenv(name);
  }

  return value;
}

void delEnvVar(char* name) {
  if(name == NULL) {
    printf("name is empty\n");
  } else {
    CHECK(unsetenv(name) != -1);
    printf("%s deleted\n", name);
  }
}