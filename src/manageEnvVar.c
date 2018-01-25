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

#include "../include/manageEnvVar.h"
#include "../include/check.h"

extern char** environ;

void listEnvVar() {
    int i = 0;
    while(environ[i]) {
        printf("%s\n", environ[i++]); 
    }
}

void addEnvVar(char* name, char* value) {
    if(name == NULL || value == NULL) {
        printf("incorrect arguments\n");
    } else {
        if(getenv(name) != NULL) {
            printf("%s overwrittened\n", name);
        } else {
            printf("%s created\n", name);
        }
        
        CHECK(setenv(name, value, 1) != -1);
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
        CHECK(unsetenv("name") != -1);
    }
}