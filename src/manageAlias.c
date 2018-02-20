/**
 * \file manageAlias.c
 * \brief All functions on alias
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date February 07, 2018 
 * 
 * File containing all the functions on environment variables
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

extern struct AliasArray *aliases;

void initAliases() {
  aliases = malloc(sizeof(*aliases));
  aliases->numAliases = 0;
  aliases->aliases = NULL;
  //TODO check Aliases
}

Alias initAlias() {
  Alias al;
  al.alias = malloc(4096 * sizeof(char)); //TODO CALCULER LA TAILLE
  al.command = malloc(4096 * sizeof(char)); //TODO CALCULER LA TAILLE
  return al;
}

//TODO http://nicolasj.developpez.com/articles/regex/#LIV-B
void manageAlias(Node *node) {
  //printf("command = %s\nargument = %s\n", node->action->command, node->action->arguments);
  if(strcmp(node->action->arguments, "") == 0)
    printAlias();
  else {

    const char *delRegex = "(^-d [[:alnum:]]+$)";
    const char *helpRegex = "(^(-h$|--help$))";
    const char *addRegex = "(^[[:alnum:]]+=.+)";
    const char *searchRegex = "(^(-s|--search)\\s[:alnum:]+)"; //strstr (contains)

    char *alias = NULL;

    if(checkRegex(addRegex, &alias, node->action->arguments)) {
      printf("ADD ALIAS : %s\n", alias);
      addAlias(alias);
    }

    if(checkRegex(delRegex, &alias, node->action->arguments)) {
      printf("DEL ALIAS : %s\n", alias);
      delAlias(alias);
    }

    if(checkRegex(searchRegex, &alias, node->action->arguments)) {
      printf("SEARCH ALIAS : %s\n", alias);
    }

    if(checkRegex(helpRegex, &alias, node->action->arguments)) {
      printAliasUsage();
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
  alias += 3;
  int c = isAliasExist(alias);
  if(c == -1) {
    printf("This alias doesn't exist\n");
    return ;
  }

  struct AliasArray tmp;
  tmp.aliases = malloc(sizeof(Alias) *(aliases->numAliases - 1));

  if(tmp.aliases) {
    int i, j = 0;
    while(i < aliases->numAliases) {
      if(i != c) {
        tmp.aliases[j] = initAlias();
        tmp.aliases[j].alias = aliases->aliases[i].alias;
        tmp.aliases[j].command = aliases->aliases[i].command;
        j++;
      }
      i++;
    }

    free(aliases->aliases);
    aliases->aliases = tmp.aliases;
    aliases->numAliases = aliases->numAliases - 1;
    free(tmp.aliases);
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

  char *aliasName = malloc(4096 * sizeof(char)); // TODO REAL SIZE
  char *aliasCommande = malloc(4096 * sizeof(char)); // TODO REAL SIZE

  getAliasInfos(alias, &aliasName, &aliasCommande);

  if(strcmp(aliasName, "") == 0 || strcmp(aliasCommande, "") == 0)
    return;

  int c = isAliasExist(aliasName);

  if(c == -1) {
    //TODO WHITESPACE
    aliases->aliases = realloc(aliases->aliases, sizeof(Alias) *(aliases->numAliases +1)); //TODO CHECK
    aliases->aliases[aliases->numAliases] = initAlias();
    aliases->aliases[aliases->numAliases].alias = aliasName;
    aliases->aliases[aliases->numAliases].command = aliasCommande;
    aliases->numAliases++;
  }
  else {
    aliases->aliases[c].command = aliasCommande;
  }

  
}

void getAliasInfos(char* alias, char** name, char** commande) {

  int i = 0;
  while(*alias != '='){
    if(*alias == 0) {
      (*name) = "";
      return;
    }

    (*name)[i] = *alias;
    ++alias;
    i++;
  }

  (*name)[i] = '\0';
  ++alias;
  
  i = 0;
  while(*alias != 0) {
    (*commande)[i] = *alias;
    ++alias;
    i++;
  }
  (*commande)[i] = '\0';
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