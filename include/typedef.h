#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

/**
 * Struct Parameters composed by all the parameters of the program
 */
typedef struct Parameters {
    char *command;
    bool verbose;
} Parameters;

/**
 * Struct Action composed by command and arguments
 */
typedef struct Action {
    char *command;
    char *arguments;
} Action;

/**
 * Struct Redirection composed by operator and file
 */
typedef struct Redirection {
    char *type;
    char *file;
} Redirection;

/**
 * Struct Node composed of a command and an operator
 */
typedef struct Node {
    char *operator;
    Action *action;
    Redirection *input;
    Redirection *output;
    Redirection *error;
    struct Node *next;
} Node;

/*
 * Struct LinkedList composed of a bool for the background mode and a node
 */
typedef struct LinkedList {
    bool isBackgrounded;
    Node *rootNode;
} LinkedList;

/**
 * Struct ForkMap with an index, a pid and a value
 */
typedef struct ForkMap {
    int index;
    int pid;
    char *value;
} ForkMap;

/**
 * Struct Alias define an alias and its command 
 */
typedef struct Alias {
    char *alias;
    char *command;
} Alias;

/**
 * Struct AliasArray define an array of alias and its command 
 */
typedef struct AliasArray {
    int numAliases;
    Alias *aliases;
} AliasArray;

#endif /** __TYPEDEF_H__ **/