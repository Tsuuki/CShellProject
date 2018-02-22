/**
 * \file regex.c
 * \brief All functions on regex
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date February 22, 2018 
 * 
 * File containing all the regex functions
 */

#include <stdbool.h>
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include "../include/reg.h"

bool checkRegex(const char* regex, char** alias, char* str) {
  regex_t preg;

  if(regcomp(&preg, regex, REG_EXTENDED) == 0) {
    size_t nmatch = 0;
    regmatch_t *pmatch = NULL;
    
    nmatch = preg.re_nsub;
    pmatch = malloc(sizeof (*pmatch) * nmatch);

    if(pmatch) {
      if(regexec(&preg, str, nmatch, pmatch, 0) == 0) {
        ssize_t size = pmatch[0].rm_eo - pmatch[0].rm_so;
        *alias = malloc(sizeof(*alias) * (size + 1));
        if(*alias) {
          strcpy(*alias,*&str);
          regfree(&preg);
          return true;
        }
      }
    }
  }
  return false;
}
