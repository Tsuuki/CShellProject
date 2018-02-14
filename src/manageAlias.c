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
    //TODO DIFFERENCIER ADD AND DELETE
    const char *delRegex = "^-d [:alnum:]+$";
    const char *helpRegex = "^(-h$|--help$)";
    const char *addRegex = "^[:alnum:]+=[:alnum:]+";
    regex_t preg;

    if(regcomp(&preg, addRegex, REG_NOSUB | REG_EXTENDED) == 0) {
      int match = 0;
      size_t nmatch = 0;
      regmatch_t *pmatch = NULL;
      printf("Regex ok\n");
      nmatch = preg.re_nsub;
      pmatch = malloc(sizeof (*pmatch) * nmatch);

      if(pmatch) {
        printf("chaine : %s\n", node->action->arguments);
        match = regexec(&preg, node->action->arguments, nmatch, pmatch, 0);
        regfree(&preg);

        printf("Match ? %d\n", match);
      }
    }
    addAlias(node);
  }
}

void addAlias(Node *node) {

  char *aliasName = malloc(4096 * sizeof(char)); // TODO REAL SIZE
  char *aliasCommande = malloc(4096 * sizeof(char)); // TODO REAL SIZE

  getAliasInfos(node->action->arguments, &aliasName, &aliasCommande);

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

  ++alias;
  
  i = 0;
  while(*alias != 0) {
    (*commande)[i] = *alias;
    ++alias;
    i++;
  }
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