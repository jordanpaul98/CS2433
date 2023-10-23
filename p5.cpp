/*
Program 5: Extending Processing of RPN, additonal functions and predefined constants
CS 2433 Fall 23
Due: Oct, 21, 2023
Completed: Oct 20, 2023
Jordan Paul, jopaul

read input from cin until #stop input is encountered. Then prints the results
of the operations indicated from stdin (expands on program 4)

constant defined by !, variable defined by $, retrieve by name
raises NameError on no match

operands (floating point values)
operators (+, -, *, /, ^, &, ~, ', %)

addtional functions:
sin (sine), cos (cosine), tan (tangent)
sqrt (squart root)
radian (degree to radians)

predfined constants:
e, eps  (exponet  2.718....)
p, pi   (pi       3.1415...)
pi2     (half pi)

compile:
g++ p5.cpp -o p5 -lm


*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

#define MAX_INPUT   8 // max character read limit
#define CONSTANT    '!'   // constant discrim
#define VARIABLE    '$'   // variable discrim

// operation types (characters)
#define OP_PLUS              '+'
#define OP_MINUS             '-'
#define OP_MULTIPLY          '*'
#define OP_DIVIDE            '/'
#define OP_POW               '^'
#define OP_DUPLICATE         '&'
#define OP_NEGATE            '~'
#define OP_INVERT            '\''
#define OP_SWAP              '%'
#define OP_SIN               's'
#define OP_COS               'c'
#define OP_TAN               't'
#define OP_SQRT              'S'
#define OP_RADIAN            'r'

// array of operation string repersentation
const char* operation_strings[5] = 
    {"sin", "cos", "tan", "sqrt", "radian"};
// array of operation string character match
const char  operation_str_match[5] = 
    {OP_SIN, OP_COS, OP_TAN, OP_SQRT, OP_RADIAN};

const char operations_chars[9] = 
    {OP_PLUS, OP_MINUS, OP_DIVIDE, OP_MULTIPLY, OP_POW,
     OP_DUPLICATE, OP_NEGATE, OP_INVERT, OP_SWAP};

// link list node(or link)
typedef struct link {
	float          *value;
    char           *type;
    char           *name;
	struct link    *head;
    struct link    *tail;      
} Link;

template <typename T>
void CHECK_MEMORY_ALLOCATION(T* ptr){
    //checks see if pointer points to valid memory allocation
    if (ptr == NULL){
        printf("Failed to allocate memory: Exitting");
        exit(1);
    }
}

void NAME_ERROR(const char* name){
    // raises NameError exception
    printf("NameError: [ %s ] not in list\n", name);
    exit(1);
}

void PREDEFINED_ERROR(const char* name){
    // raises name already defined error
    printf("Constant with name [ %s ] already defined!", name);
    exit(1);
}

void CHECK_MUTABLE(Link *tlink){
    // check see if value changeable, else raise error
    if (*(tlink->type) == CONSTANT){
        printf("%s is a constant and cannot be modifed\n", tlink->name);
        exit(1);
    }
}

void INDEX_OUT_OF_BOUNDS(){
    // raises index out of bounds error
    printf("Index out of Bounds\n");
    exit(1);
}

Link* find(Link *links, const char* name){
    // find the link by name, else return NULL
    link *tlink = links;
    while (tlink != NULL && (strcmp(tlink->name, name) != 0)){
        tlink = tlink->tail;
    }
    return tlink;
}

Link* retreive(Link *links, const char* name){
    // get the link by name, else raise NameError
    link *tlink = find(links, name);
    if (tlink == NULL) NAME_ERROR(name);
    return tlink;
}

float pop(Link *&links){
    // pop top of stack
    if (links == NULL){
        printf("Stack Underflow Error\n");
        exit(1);
    }

    float val = *(links->value);
    Link *link_increment = links->tail;
    if (link_increment != NULL)
        link_increment->head = NULL;

    free(links->value);
    free(links->name);
    free(links->type);
    free(links);

    links = link_increment;

    return val;
}

void put(Link *&links, const char* name, float value, char type){
    // put a value by name into the link list and declare it's value type
    Link *temp_link;

    temp_link = (Link *)malloc(sizeof(Link));
    CHECK_MEMORY_ALLOCATION(temp_link);
    temp_link->value = (float*)malloc(sizeof(float));
    CHECK_MEMORY_ALLOCATION(temp_link->value);
    temp_link->name = (char*)malloc(MAX_INPUT);
    CHECK_MEMORY_ALLOCATION(temp_link->name);
    temp_link->type = (char*)malloc(sizeof(char));
    CHECK_MEMORY_ALLOCATION(temp_link->type);

    *(temp_link->value) = value;
    strcpy(temp_link->name, name);
    *(temp_link->type) = type;

    temp_link->tail = links;
    if (links != NULL) links->head = temp_link;

    links = temp_link;
}

void put(Link *&links, float value){
    // put a value on a stack
    put(links, "", value, VARIABLE);
}

float get(Link *links, const char* name){
    // get the value of a link by name
    return *(retreive(links, name)->value);
}

void set(Link *links, const char* name, float value){
    // set the value of a link by name
    Link *tlink = retreive(links, name);
    CHECK_MUTABLE(tlink);
    *(tlink->value) = value;
}

char isOperation(const char* op){
    // check see if passed name is a operation, return operation character
    if (strlen(op) == 1) {
        for (int c = 0; c < sizeof(operations_chars) / sizeof(char); c++){
            if (op[0] == operations_chars[c]) return op[0];
        }
    }else{
        for (int c = 0; c < sizeof(operation_str_match) / sizeof(char); c++){
            if (strcmp(op, operation_strings[c]) == 0) 
                return operation_str_match[c];
        }
    }
    return 0;
}

void validName(const char* intext){
    // check naming convention of a entered variable or constant
    if (intext[0] == '\0'){
        printf("Variable or Constant must have name behind type\n");
        exit(1);
    }
    if (isOperation(intext) > 0) {
        printf("Variable or Constant [ %s ] name cannot be operation type\n", intext);
        exit(1);
    }
    if (isalpha(intext[0]) == 0) {
        printf("Variable or Constant [ %s ] name must start with a alpha character\n", intext);
        exit(1);
    }
    for (int i = 0; i < strlen(intext); i++){
        if (! ((isalpha(intext[i]) > 0) || (isdigit(intext[i]) > 0))){
            printf("Variable or Constant [ %s ] name can only contain alpha or numerical characters\n", intext);
            exit(1);
        }
    }
}

void strip(char *name){
    //strip first index of a char array
    int len = strlen(name);
    for (int i = 0; i < len - 1; i++) {
        name[i] = name[i + 1];
    }
    name[len - 1] = '\0';
}

int main(int argc, char* argv[]){

    float fpv;
	char intext[MAX_INPUT];
    intext[MAX_INPUT - 1] = '\0';
    float A; float B;
    int loop_hold = 1;

    Link *operands = NULL;
    Link *variables = NULL;

    put(variables, "pi",    M_PI,   CONSTANT);
    put(variables, "p",     M_PI,   CONSTANT);
    put(variables, "pi2",   M_PI_2, CONSTANT);
    put(variables, "eps",   M_E,    CONSTANT);
    put(variables, "e",     M_E,    CONSTANT);

    while (loop_hold){

        cin >> intext;
        if (intext[MAX_INPUT - 1] != '\0') {
            printf("Input Buffer Overflow: Max input size - %i", MAX_INPUT);
            exit(1);
        }
        if (strcmp(intext, "#stop") == 0) break;

        if (sscanf(intext, "%f", &fpv) == 0){

            if (find(variables, intext) !=  NULL){

                put(operands, get(variables, intext));

            }else if (intext[0] == CONSTANT || intext[0] == VARIABLE){

                char type = intext[0];
                strip(intext);
                validName(intext);
                if (find(variables, intext) == NULL) {
                    put(variables, intext, pop(operands), type);
                }else if (type == VARIABLE){
                    set(variables, intext, pop(operands));
                }else{
                    PREDEFINED_ERROR(intext);
                }

            }else if (isOperation(intext) > 0){
                A = pop(operands);
                switch (isOperation(intext)){
                    case OP_PLUS:
                        put(operands, A + pop(operands));
                        break;
                    case OP_MINUS:
                        put(operands, pop(operands) - A);
                        break;
                    case OP_MULTIPLY:
                        put(operands, A * pop(operands));
                        break;
                    case OP_DIVIDE:
                        if (A == 0.0) { // check for A == 0.0 to prevent float by 0 exception. Exit if so
                            printf("Division by Zero Error!");
                            exit(1);
                        }
                        put(operands, pop(operands) / A);
                        break;
                    case OP_POW:
                        put(operands, pow(pop(operands), A));
                        break;
                    case OP_DUPLICATE:
                        put(operands, A);
                        put(operands, A);
                        break;
                    case OP_NEGATE:
                        put(operands, -A);
                        break;
                    case OP_INVERT:
                        if (A == 0.0) {
                            printf("Inversion Error! Division by Zero");
                            exit(1);
                        }
                        put(operands, 1.0 / A);
                        break;
                    case OP_SWAP:
                        B = pop(operands);
                        put(operands, A);
                        put(operands, B);
                        break;
                    case OP_COS:
                        put(operands, cos(A));
                        break;
                    case OP_SIN:
                        put(operands, sin(A));
                        break;
                    case OP_TAN:
                        put(operands, tan(A));
                        break;
                    case OP_SQRT:
                        put(operands, sqrt(A));
                        break;
                    case OP_RADIAN:
                        put(operands, A * M_PI / 180.0);
                        break;
                }
            }else {
                NAME_ERROR(intext);
            }
        }else{
            //cout << "Float input: " << fpv << endl;
            put(operands, fpv);
        }
    }

    if (operands == NULL){
        printf("Emtpy Stack!\n");
    } else {
        while (operands != NULL){
            printf("%f", pop(operands));
                
            if (operands != NULL){
                printf("  ");    // if operands isnt NULL add two spaces
            }
        }
    }

    while (variables != NULL){
        pop(variables);
    }

    return 0;
    // program done
}
