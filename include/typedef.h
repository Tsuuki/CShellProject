/**
 * Struct Parameters composed by all the parameters of the program
 */
typedef struct Parameters {
    char *command;
    bool verbose;
} Parameters;

/**
 * Struct Parameters composed by all the parameters of the program
 */
typedef struct Action {
    char *command;
    char *arguments;
} Action;

/**
 * Struct Node composed of a command and an operator
 */
typedef struct Node {
    char *operator;
    Action *action;
    struct Node *next;
} Node;

/**
 * Struct Tree composed of the root node and if the command must be launched in background 
 */
typedef struct Tree {
    bool isBackgrounded;
    Node *rootNode;
} Tree;