/**
 * Struct Parameters composed by all the parameters of the program
 */
typedef struct Parameters {
    char* command;
    bool verbose;
} Parameters;

/**
 * Struct Parameters composed by all the parameters of the program
 */
typedef struct Action {
    char* command;
    char* arguments;
} Action;

/**
 * Struct Node composed of a command and an operator
 */
typedef struct Node {
    char* operator;
    struct Action* action;
    struct Node* next;
} Node;