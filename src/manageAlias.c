/**
 * \file manageAlias.c
 * \brief All functions on alias
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date February 07, 2018 
 * 
 * File containing all the alias functions
 */

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <sys/types.h>
#include <ctype.h>

#include "../include/typedef.h"
#include "../include/manageAlias.h"
#include "../include/check.h"

extern struct AliasArray *aliases;

void initAliases() {
  aliases = malloc(sizeof(*aliases));

  if(!aliases) {
    CHECK(aliases != NULL);
    exit(EXIT_FAILURE);
  }

  aliases->numAliases = 0;
  aliases->aliases = NULL;
}

void manageAlias(Node *node) {
  if(strcmp(node->action->arguments, "") == 0)
    printAlias();
  else {

    const char *delRegex = "(^-d [[:alnum:]]+$)";
    const char *helpRegex = "(^(-h$|--help$))";
    const char *addRegex = "(^[[:alnum:]]+=.+)";
    const char *searchRegex = "(^(-s|--search)\\s[[:alnum:]]+)";

    char *alias = NULL;

    if(checkRegex(addRegex, &alias, node->action->arguments)) {
      addAlias(alias);
    }
    else if(checkRegex(delRegex, &alias, node->action->arguments)) {
      delAlias(alias);
    }
    else if(checkRegex(searchRegex, &alias, node->action->arguments)) {
      searchAlias(alias);
    }
    else if(checkRegex(helpRegex, &alias, node->action->arguments)) {
      printAliasUsage();
    }
    else {
      dprintf(1, "%-8s %s\n%-8s",
          "alias : invalid option:", node->action->arguments,
          "enter \"alias --help\" for more information.\n");
    }
  }
}

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

void delAlias(char* alias) {
  // Remove "-d "
  alias += 3;
  int c = isAliasExist(alias);
  if(c == -1) {
    printf("This alias doesn't exist\n");
    return ;
  }

  // Create temporary array with size - 1
  struct AliasArray tmp;
  tmp.aliases = malloc(sizeof(Alias) *(aliases->numAliases - 1));

  if(tmp.aliases) {
    int i, j = 0;
    while(i < aliases->numAliases) {
      if(i != c) {
        tmp.aliases[j].alias = aliases->aliases[i].alias;
        tmp.aliases[j].command = aliases->aliases[i].command;
        j++;
      }
      i++;
    }

    free(aliases->aliases);
    aliases->aliases = tmp.aliases;
    aliases->numAliases = aliases->numAliases - 1;
  }
}

void searchAlias(char* alias) {
  //Remove "-s " or "--search "
  while(*alias != ' '){
    alias++;
  }
  alias++;
  for(int i = 0; i < aliases->numAliases; i++) {
    if(strstr(aliases->aliases[i].alias, alias) != NULL) {
      printf("alias %s='%s'\n",aliases->aliases[i].alias, aliases->aliases[i].command);
    }
  }
}

void printAliasUsage() {
  dprintf(1, "%-8s %s\n"
    "%-8s %s\n"
    "%-8s %s\n"
    "CRUD operation on alias.\n\n"
    "%-20s %s\n"
    "%-20s %s\n"
    "%-20s %s\n",
      "USAGE:", "alias NAME=COMMANDE",
      "  or:", "alias [OPTION] NAME",
      "  or:", "alias [OPTION]",
      "  -d","delete the given alias from aliases",
      "  -h, --help","display this help and exit",
      "  -s, --search","search the given alias in aliases");
}

void addAlias(char *alias) {

  char *aliasName;
  char *aliasCommand;

  getAliasInfos(alias, &aliasName, &aliasCommand);

  if(strcmp(aliasName, "") == 0 || strcmp(aliasCommand, "") == 0)
    return;

  int c = isAliasExist(aliasName);

  if(c == -1) {
    aliases->aliases = realloc(aliases->aliases, sizeof(Alias) *(aliases->numAliases +1));

    if(!aliases->aliases) {
      CHECK(aliases->aliases != NULL);
      return;
    }

    aliases->aliases[aliases->numAliases].alias = aliasName;
    aliases->aliases[aliases->numAliases].command = aliasCommand;
    aliases->numAliases++;
  }
  else {
    aliases->aliases[c].command = aliasCommand;
  }
}

void getAliasInfos(char* alias, char** aliasName, char** aliasCommand) {

  // First, calculate size of alias to allocate memory
  int i = 0;
  while(alias[i] != '='){
    i++;
  }

  // Allocate name + '\0'
  *aliasName = malloc(sizeof(char) * (i + 1));
  // Allocate command + '\0' - name - '='
  *aliasCommand = malloc(sizeof(char) * (strlen(alias) - i));

  // Then copy name and command
  i = 0;
  while(*alias != '='){
    if(*alias == 0) {
      (*aliasName) = "";
      return;
    }

    (*aliasName)[i] = *alias;
    ++alias;
    i++;
  }

  (*aliasName)[i] = '\0';
  ++alias;
  
  i = 0;
  while(*alias != 0) {
    (*aliasCommand)[i] = *alias;
    ++alias;
    i++;
  }
  (*aliasCommand)[i] = '\0';
}

void printAlias() {
  if(aliases->numAliases == 0) {
    printf("No alias registered\n");
    return;
  }

  for(int i = 0; i < aliases->numAliases; i++) {
    printf("alias %s='%s'\n",aliases->aliases[i].alias, aliases->aliases[i].command);
  }
}

int isAliasExist(char* name) {
  for(int i = 0; i < aliases->numAliases; i++) {
    if(strcmp(aliases->aliases[i].alias, name) == 0) {
      return i;
    }
  }
  return -1;
}