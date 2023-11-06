/*
Program 6: C++ and cin, additional functions and predefined constants
CS 2433 Fall 23
Due: Nov 04, 2023
Completed: Nov 2, 2023
Jordan Paul, jopaul

read input from cin until #stop input is encountered. Then prints the results
of the operations indicated from stdin (expands on program 4 and program 5)

Replace struct implementation with C++ class, class will maintain links to their
predecessors and successors but stack management will be handled by an
unordered_map and retrieve by name

constant defined by !, variable defined by $, retrieve by name
raises NameError on no match

additional functions: @ sign
    - @! will pop and print top of the operands stack
    - @<var> will print variable or constant to newline
    - @"<any non-whitespace text> will print out into newline

operands (double values)
operators (+, -, *, /, ^, &, ~, ', %)

-sqrt (squart root)
-sin (sine), cos (cosine), tan (tangent)
-radian (degree to radians), degrees (radians to degrees)
NOTE: sin, cos, and tan function outputs assume input angle x in radians as so the RPN
      expects a user to pass radian input into these functions

      using the input: 180 radians cos -> will produce the expect output if wanting in degrees

predfined constants:
e, eps  (exponet  2.718....)
p, pi   (pi       3.1415...)
pi2     (half pi)

compile:
g++ p6.cpp -o p6 -lm

handin code:
handin cs2433-rlchurc program6 p6.cpp

*/

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unordered_map>

#define MAX_INPUT 128

#define CONSTANT_TYPE       '!'
#define VARIABLE_TYPE       '$'
#define EXPRESSION_TYPE     '@'

#define EXPRESSION_NOTE     '"'
#define EXPRESSION_POP      '!'

using namespace std;

const char* constants[5] = {
        "pi", "p", "pi2", "eps", "e"
};
const double const_values[5] = {
        M_PI, M_PI, M_PI_2, M_E, M_E
};

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
#define OP_RADIANS           'r'
#define OP_DEGREES           'd'

// array of operation string repersentation
const char* operation_strings[6] =
        {"sin", "cos", "tan", "sqrt", "radians", "degrees"};
// array of operation string character match
const char  operation_str_match[6] =
        {OP_SIN, OP_COS, OP_TAN, OP_SQRT, OP_RADIANS, OP_DEGREES};

const char operations_chars[9] =
        {OP_PLUS, OP_MINUS, OP_DIVIDE, OP_MULTIPLY, OP_POW,
         OP_DUPLICATE, OP_NEGATE, OP_INVERT, OP_SWAP};

// link class
class Link{
private:
    // private only to this class, access by callables
    double value;
    char type;         //link type Constant or Variable
    char name[MAX_INPUT];       //link name
    Link *head, *tail; //predecessors and successors

public:
    Link(double value, const char* name, char type=VARIABLE_TYPE)
            : value(value)
            , type(type)
            , head(NULL)
            , tail(NULL) { strcpy(this->name, name); }

    //set successor
    void setHead(Link &head) { this->head = &head; }

    //set predecessor
    void setTail(Link &tail) { this->tail = &tail; }

    // get successor
    Link* getHead() { return this->head; }

    // get predecessor
    Link* getTail() { return this->tail; }

    // retrieve ending of the link
    Link* end() { return this->tail == NULL ? this : this->tail->end(); }

    // retrieve beginning of the link
    Link* begin() { return this->head == NULL ? this : this->head->begin(); }

    // get link value
    double getValue() { return value; }

    // get link Name
    char* getName() { return name; }

    char getType(){ return type; }

    // set the link value: returns 1 if value was not set (is a constant)
    int setValue(double value) {
        if(this->type != CONSTANT_TYPE){
            this->value = value;
            return 0;
        }
        return 1;
    }

    // print the link chain!
    void printLinks() {
        cout << this->name << ": " << this->value << endl;
        if (this->tail != NULL) getTail()->printLinks();
    }
};

void put(unordered_map<unsigned int, Link>& iMap, double value){
    // helper function to put a value onto top of operands stack
    // unordered_map ref by number - map.size() keeps the order
    Link tLink(value, "");

    if (iMap.size() > 0) { // set the successor and predecessor
        iMap.at(iMap.size() - 1).setHead(tLink);
        tLink.setTail(iMap.at(iMap.size() - 1));
    }
    iMap.emplace(iMap.size(), tLink);
}

double pop(unordered_map<unsigned int, Link> &iMap){
    if (iMap.size() == 0){
        printf("Stack underflow Error!\n");
        exit(1);
    }
    double value = iMap.at(iMap.size() - 1).getValue();
    iMap.erase(iMap.size() - 1);
    return value;
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

const char* headName(unordered_map<string, Link>& iMap){
    // helper function to get name of last element added to variable stack
    if (iMap.size() == 0)
        { return ""; }
    // between here to - string varHead = headName(vairables);
    return iMap.at(iMap.begin()->first).begin()->getName();
}

void validName(const char* intext){
    // check naming convention of a entered variable or constant see if it's valid
    if (intext[0] == '\0'){
        printf("Variable or Constant must have name behind type\n");
        exit(1);
    }
    if (isalpha(intext[0]) == 0) {
        printf("Variable or Constant [ %s ] name must start with a alpha character\n", intext);
        exit(1);
    }
    if (isOperation(intext) > 0) {
        printf("Variable or Constant [ %s ] name cannot be operation type\n", intext);
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

double divide(double n, double d){
    // division function which handles division by zero
    if (d == 0.0) {
        cout << "Divide by Zero Error: division by zero";
        exit(1);
    }
    return n / d;
}

int main(int argc, char* argv[]){

    // variable declaration
    double fpv;
    char intext[MAX_INPUT];
    intext[MAX_INPUT - 1] = '\0';
    double A; double B;
    int i;

    // operands and variable stack
    unordered_map<string, Link> variables;
    unordered_map<unsigned int, Link> operands;

    Link tLink(0, "", 0);

    // add pre-defined constants to variables and point links between them
    for (i=0; i<sizeof(const_values)/sizeof(double); i++){
        Link tLink(const_values[i], constants[i], CONSTANT_TYPE);
        variables.emplace(constants[i], tLink);
        if (i > 0){
            variables.at(constants[i]).setTail(variables.at(constants[i - 1]));
            variables.at(constants[i - 1]).setHead(variables.at(constants[i]));
        }
    }

    //check see if links work. get the initial link (end), navigate up to head (begin), then print down the chain
    //cout << endl; variables.at(constants[0]).begin()->printLinks(); cout << endl;
    //cout << endl; variables.at(constants[4]).printLinks(); cout << endl;

    while (-1) {
        cin >> intext;
        if (intext[MAX_INPUT - 1] != '\0') {
            printf("Input Buffer Overflow: Max input size - %i", MAX_INPUT);
            exit(1);
        }
        //break, nothing else to do
        if (strcmp(intext, "#stop") == 0) break;

        // check if input is a string, else fill the double value
        if (sscanf(intext, "%lf", &fpv) == 0){

            if (variables.find(intext) != variables.end()){
                // put variable into operands stack if variable available
                operands.emplace(operands.size(), variables.at(intext));
            
            }else if(intext[0] == CONSTANT_TYPE || intext[0] == VARIABLE_TYPE){
                // process Constants(!) and Variable($) if input starts with ! or $
                char type = intext[0];
                strip(intext);
                validName(intext);

                if(variables.find(intext) == variables.end()){
                    // process Constants(!) and Variable($) if input starts with ! or $
                    const char *name = headName(variables);

                    //cout << "Appending variable: " << intext << "    Current Head: " << name << endl;

                    Link tLink(pop(operands), intext, type);

                    variables.emplace(intext, tLink);

                    if (name[0] != '\0'){
                        variables.at(intext).setTail(variables.at(name));
                        variables.at(name).setHead(variables.at(intext));
                    }

                    //variables.at(intext).printLinks();
                }else if ( type == CONSTANT_TYPE ){
                    printf("Predefined Error: [%s] already exist", intext);
                    exit(1);
                } else {
                    variables.at(intext).setValue(pop(operands));
                }
            }else if(intext[0] == EXPRESSION_TYPE){
                // process output text if input starts with @
                strip(intext);
                if (variables.find(intext) != variables.end()){
                    cout << variables.at(intext).getValue() << endl;
                }else if (intext[0] == EXPRESSION_POP){
                    strip(intext);
                    cout << pop(operands) << endl;
                }else if (intext[0] == EXPRESSION_NOTE){
                    strip(intext);
                    cout << intext << endl;
                }else{
                    printf("NameError: [ %s ] not in list\n", intext);
                    exit(1);
                }

            }else if (isOperation(intext) > 0){
                // process operations (isOperations returns a char if input is an operation type else 0)
                A = pop(operands);

                switch (isOperation(intext)) {
                    case OP_PLUS:       put(operands, pop(operands) + A); break;
                    case OP_MINUS:      put(operands, pop(operands) - A); break;
                    case OP_MULTIPLY:   put(operands, pop(operands) * A); break;
                    case OP_DIVIDE:     put(operands, divide(pop(operands), A)); break;
                    case OP_POW:        put(operands, pow(pop(operands), A)); break;
                    case OP_DUPLICATE:  put(operands, A); put(operands, A); break;
                    case OP_NEGATE:     put(operands, -A); break;
                    case OP_INVERT:     put(operands, divide(1.0, A)); break;
                    case OP_SWAP:       B = pop(operands);
                                        put(operands, A); put(operands, B); break;
                    case OP_COS:        put(operands, cos(A)); break;
                    case OP_SIN:        put(operands, sin(A)); break;
                    case OP_TAN:        put(operands, tan(A)); break;
                    case OP_SQRT:       put(operands, sqrt(A)); break;
                    case OP_RADIANS:    put(operands, A * M_PI / 180.0); break;
                    case OP_DEGREES:    put(operands, A * 180.0 / M_PI); break;
                    default: break;
                }
            }else{
                // no operation was matched, most likely as variable call was requested. print error
                printf("NameError: [ %s ] not in list\n", intext);
                exit(1);
            }
        }else{
            // if not string, put into the operand stack
            put(operands, fpv);
        }
    }

    // see if new variables added were linked correctly
    //cout << endl; variables.at(constants[0]).begin()->printLinks(); cout << endl;

    // print the operands stack
    if (operands.size() > 0) {
        while (operands.size() > 0) {
            cout << setprecision(5) << fixed << operands.at(operands.size() - 1).getValue() << (operands.size() > 0 ? "  " : "");
            operands.erase(operands.size() - 1);
        }
    }else{
        cout << "Empty Stack!" << endl;
    }

    cout << endl;

    return 0;
    // end program
}
