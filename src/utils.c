/**
 * \file utils.c
 * \brief Util file.
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date January 26, 2018 
 * 
 * Contain all the utils functions
 */
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../include/utils.h"
#include "../include/manageEnvVar.h"

// FREE MEMORY ??
char* getRootPermission() {
  uid_t uid=getuid(), euid=geteuid();
  if (uid<=0 || uid!=euid) {
      return "#";
  }

  return "$";
}

// FREE MEMORY ??
char* getUserHostName() {
  char* hostname = malloc(1024 * sizeof(char));
  gethostname(hostname, 1024 * sizeof(char)); // WHY ??
  return hostname;
}

// FREE MEMORY ??
char* getUserName() {
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  if (pw)
  {
    return pw->pw_name;
  }

  return "guest";
}

// FREE MEMORY ??
char* getWorkingDirectory() {
  char* cwd = malloc(1024 * sizeof(char));
  getcwd(cwd, sizeof(char) * 1024);
 
  return cwd;
}

// You must free the result if result is non-NULL. CHECK THIS
char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

void clean(const char *buffer, FILE *fp){
    
    char *p = strchr(buffer,'\n');

    if (p != NULL)
        *p = 0;
    else {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }

}

void freeIfNeeded(void* toFree) {
  if (toFree != NULL) free(toFree);  
}