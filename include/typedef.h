/**
 * Struct Parameters composed by all the parameters of the program
 */
typedef struct {
    char* command;
    bool verbose;
} Parameters;

/**
 * Struct Node composed of a command and an operator
 */
typedef struct {
    char* value;
    struct Node* command;
    struct Node* operator;
} Node;

/**
 * Struct tree composed with a root node
 */
typedef Node tree;