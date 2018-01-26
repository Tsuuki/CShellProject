/**
 * Struct Parameters composed by all the parameters of the program
 */
typedef struct Parameters {
    char* command;
    bool verbose;
} Parameters;

/**
 * Struct Node composed of a command and an operator
 */
typedef struct Node{
    char* operator;
    char* command;
    struct Node* nextNode;
} Node;