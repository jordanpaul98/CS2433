/*
Program 4: Extending Processing of RPN, plus variable and Constants
Jordan Paul - Fall 2023

read input from stdin until #stop input is encountered. Then prints the results
of the operations indicated from stdin

operands (floating point values)
operators (+, -, *, /, ^, &, ~, ', %)
^: power- pop top = B, pop again = A, push A ^ B onto stack
&: duplication- top of stack and push both back onto operands stack
~: negate- pop and push value as negative onto stack
': invert- pop top and push 1 / pop on stack. Raises divide by zero error
%: swap- top of stack, pop top = A, pop again = B, push A, push B

Constant: if a String is read that starts with a !, pop the stack and create a constant from that value that cannot be changed
Variable: if a String is read that starts with a $, pop the stack and create a variable from that value that can be changed if the name exist

ByName: if a string is passed without (! or $) and it exist in the within the Variable or Constant stack, push that value onto the Operands Stack

raise Name Error if attempting the set a constant that already been declared
raise Index Error if attempthing a get variable by name that doesnt exist

implement head and tail pointers for each node

link that is called by name with be treated as a Dictionary, else link is treated as a push, pop array

Additional Features:
--help - shows help items
--debug: enabled printing of program operations
--results: prints the output of the funciton
#stack: prints the operands stack
#vars: prints the variable and constant stack
#pop: pops top of the stack

compile: gcc p4.c -o p4 -lm

*/

// standard librarys for strings, math, and types
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT   64 // max character read limit
#define CONSTANT 2 // constant discrim
#define VARIABLE 3 // variable discrim
int DEBUG_MODE = 0; // print program operations
int PRINT_RESULTS = 0; // print output of math operation

char operations_types[9] = {'+', '-', '*', '/', '^', '&', '~', '\'', '%'};

// link list node(or link)
typedef struct link {
	float *value;  // value of the link
    int *type;     // link type when using link as a constant or variable
    char *name;    // link name when using link as a constant or variable
	struct link *head; // next link of the list
    struct link *tail; // previous link of the list
} nlink;

struct link *operands = NULL;   // initial link head
struct link *vars = NULL; // intital variable or constant head

nlink* dictionary(char *name){
    // funciton to create a and allocate memory fopr new link for a constant or variable
    if (DEBUG_MODE) printf("Creating a link associated with name: [ %s ]\n", name);

    if (*name == '\0') {
        printf("NameError: Variable or constant name cannot be NULL");
        exit(1);
    }

    nlink* temp_link;
    temp_link = (nlink*)malloc(sizeof(nlink));
    if (temp_link == NULL) {                    // check make sure memory was allocated
        printf("Failed to allocate memory\n");
        exit(1);
    }

    temp_link->name = (char *)malloc(sizeof(name));
    if (temp_link->name == NULL){
        printf("Failed to allocate memory for dictionary name\n");
        exit(1);
    }
    temp_link->value = (float *)malloc(sizeof(float));
    if (temp_link->value == NULL) {    //check make sure memory was allocated
        printf("Failed to allocate memory for value\n");
        exit(1);
    }

    temp_link->type = (int *)malloc(sizeof(int));
    if (temp_link->type == NULL){
        printf("Failed to allocate memory for variable type\n");
        exit(1);
    }
    
    // copy name to the link
    strcpy(temp_link->name, name);

    return temp_link;
}

void nameError(nlink *link){
    // Raise error if name of a link is not valid
    if (link->name == NULL || *link->name == '\0'){
        printf("NameError: Link name contained a NULL name");
        exit(1);
    }
}

void variable(struct link *var, float value, int type){
    // build the variable type and assign its value and setup pointers to the variable list
    if (DEBUG_MODE) {
        printf("Assinging Variable [ %s ] to a ", var->name);
        if (type == CONSTANT){ printf("Constant with value: %f\n", value);
        }else{ printf("Variable with value: %f\n", value); }
    }

    // make sure the variable or constant is named
    nameError(var);

    *(var->value) = value;
    *(var->type) = type;

    var->head = vars;
    if (vars != NULL) {
        vars->tail = var;
    }
    vars = var;
}

void put(float val){
    struct link *temp_link;                     // temp link to put at the head
    temp_link = (nlink *)malloc(sizeof(nlink)); // allocate memory for link

    if (temp_link == NULL) {                    // check make sure memory was allocated
        printf("Failed to allocate memory\n");
        exit(1);
    }

    //  allocate memory for the float value that link value will point to
    temp_link->value = (float *)malloc(sizeof(float));
    if (temp_link->value == NULL) {    //check make sure memory was allocated
        printf("Failed to allocate memory\n");
        exit(1);
    }

    *(temp_link->value) = val; // set and point link to float location
    
    temp_link->head = operands; // set temp link to point to previous link
    if (operands != NULL) operands->tail = temp_link; // set current list head tail to new head if its a link
    operands = temp_link;       // set operands to new link in chain

    if (DEBUG_MODE) printf("Value added to stack: %f\n", *(temp_link->value));
}

void freeLink(struct link *link) {  // free link when when not used anymore
    if (link != NULL) {
        free(link->value);  // Free the float value
        free(link);         // Free the link struct
    }
}

float pop(){
    if (operands == NULL){ // make sure we actually have a value to pop!
        printf("Stack Underflow Error!\n");
        exit(1);
    }

    float val = *(operands->value); // create local variable to store head link list value
    struct link *stack = operands->head; // set a temp link pointing to next link in the list

    freeLink(operands); // free the head of the link list

    operands = stack; // set the operands(head) to temp link
    if (operands != NULL) operands->tail = NULL; // set the tail to NULL as it's the new head

    if (DEBUG_MODE) printf("Popped Value from Stack!: %f\n", val);
    return val;  // return the float value popped from the head of the list
}

float peek(){
    // get value from head of the stack
    if (operands == NULL) return 0;

    return *(operands->value);
}

float get(nlink *link, char *name){
    // for dictionary style list: get value by name. need the head of list and name
    struct link *loop_link = link;
    while (loop_link != NULL){
        nameError(link); // check see if name value

        if (strcmp(loop_link->name, name) == 0){
            return *(loop_link->value);
        }
        loop_link = loop_link->head;
    }
    printf("IndexError: [ %s ] not in list", name); // raise error if name doesnt exist
    exit(1);
}

void set(nlink *link, char *name, float value){
    // for dictionary style list: set value by name. need the head of list, name and value
    struct link *loop_link = link;
    while (loop_link != NULL){
        nameError(link); // check see if name value

        if (strcmp(loop_link->name, name) == 0){
            *(loop_link->value) = value;
            return;
        }
        loop_link = loop_link->head;
    }
    printf("IndexError: [ %s ] not in list", name); //raise error if name doesnt exist
    exit(1);
}

int find(nlink *link, char *name){
    // find the index where the name link exist
    struct link *loop_link = link;
    int _index = 0;
    while (loop_link != NULL){
        nameError(link); // check see if name value

        if (strcmp(loop_link->name, name) == 0){
            return _index;
        }

        _index ++;
        loop_link = loop_link->head;
    }
    return -1; //return -1 if name is not in list
}

void printStack(){
    // prints the stack without modifying anything
    struct link *temp_operands = operands;
    if (temp_operands == NULL) { // if operands is NULL then nothings in the stack!
        printf("Nothing in Stack to Print!\n");
    }else {
        int count = 0;
        printf("-Stack contents and index-\n");
        while(temp_operands != NULL){ // else loop over each element pop then print!
            printf("[%i] %f\n", count, *(temp_operands->value));
            count++;
            temp_operands = temp_operands->head;
        }
        printf("\n");
    }
}

void printVariables(){
    struct link *temp_vars = vars;
    if (temp_vars == NULL) { // if operands is NULL then nothings in the stack!
        printf("Nothing in Stack to Print!\n");
    }else {
        printf("-Variables type, names, and values-\n");
        while(temp_vars != NULL){ // else loop over each element pop then print!
            if (*temp_vars->type == CONSTANT) {
                printf("[CONSTANT] - [ %s ] %f\n", temp_vars->name, *(temp_vars->value));
            } else {
                printf("[VARIABLE] - [ %s ] %f\n", temp_vars->name, *(temp_vars->value));
            }
            temp_vars = temp_vars->head;
        }
        printf("\n");
    }
}

void strip(char *name){
    //strip first index of a char array
    if (DEBUG_MODE) printf("Striping first variable from: %s\n", name);
    if (name == NULL || *name == '\0') {
        return;
    }

    int len = strlen(name);
    for (int i = 0; i < len - 1; i++) {
        name[i] = name[i + 1];
    }
    name[len - 1] = '\0';
}

void help(){
    printf("RNP Calculator Usage\n");
    printf("    #stop     - stop the program\n");
    printf("    #vars     - print the variable and constant stack\n");
    printf("    #stack    - print the operands stack\n");
    printf("    #pop      - pop the top of the operands stack\n");
    printf("    ![name]   - pop top value of operands stack and create a named constant from it\n");
    printf("    $[name]   - pop top valye of operands stack and create a named variable from it\n");
    printf("    [name]    - get variable or constant by name and add it to the operands stack\n");
    printf("              * current character readable limit: %i\n", MAX_INPUT);
    printf("    --debug   - enable program progression printing\n\n");
    printf("    --results - prints the results of operations");
    printf("    Available Operators [+, -, *, /, ^, &, ~, \', %%]\n");
    printf("    +: Addition\n"); printf("    -: Substraction\n"); printf("    *: Multiplication\n"); printf("    /: Division\n");
    printf("    ^: Power\n"); printf("    &: Duplicate\n"); printf("    ~: Negate\n"); printf("    \': invert\n"); printf("    %%: swap\n\n");
}

void toggleDebug(){
    // toggle the debug mode
    if (DEBUG_MODE){
        DEBUG_MODE = 0;
        printf("[PRGM] Debugging Disabled\n");
    }else{
        DEBUG_MODE = 1;
        printf("[PRGM] Debugging Enabled\n");
    }
}

void togglePrintResults(){
    //toggle the print results
    if (PRINT_RESULTS){
        PRINT_RESULTS = 0;
        printf("[PRGM] Result Printing Disabled\n");
    }else{
        PRINT_RESULTS = 1;
        printf("[PRGM] Result Printing Enabled\n");
    }
}

int main(int argc, char* argv[]){

	float fpv;					// float input value
	char intext[MAX_INPUT];		// char array input value. set by max characters
    intext[MAX_INPUT - 1] = '\0';

	struct link *temp_link;     // temporary link pointer

    for (int i = 1; i < argc; i++){ // look for startup flags
        if (strcmp(argv[i], "--debug") == 0 && DEBUG_MODE == 0){
            DEBUG_MODE = 1;
            printf("[PRGM] Debugging Enabled\n");
        }else if (strcmp(argv[i], "--resutls") == 0 && PRINT_RESULTS == 0){
            PRINT_RESULTS = 1;
            printf("[PRGM] Result Printing Enabled\n");
        }
    }

	while (1){                  // while loop until #stop encountered
		scanf("%s", intext);    // read next input or white space

        if (intext[MAX_INPUT - 1] != '\0'){ //make sure the input string is valid
            printf("Maximum string input reached: Current limit- %i characters", MAX_INPUT);
            exit(1);
        } else if (strcmp(intext, "#stop") == 0){ // nothing else to do. Break!
            break;
        } else if (strcmp(intext, "--help") == 0){ // print help message
            help();
            continue;
        } else  if (strcmp(intext, "#stack") == 0){ // print orerands stack
            printStack();
            continue;
        } else if (strcmp(intext, "#vars") == 0){ // print variable stack
            printVariables();
            continue;
        } else if (strcmp(intext, "--debug") == 0){ // enable/disable debugging
            toggleDebug();
            continue; 
        } else if (strcmp(intext, "--results") == 0){ // enable/disable result printing
            togglePrintResults();
            continue;
        } else if (strcmp(intext, "#pop") == 0){ // pop top of stack
            pop();
            continue;
        }

		if (sscanf(intext, "%f", &fpv) == 1){ // if value value was read, put it into the operands stack
			put(fpv);
		}else { // else preform a operation

            if (intext[0] == '!'){ // constant creating
                if (DEBUG_MODE) printf("Creating a constant from top stack\n");
                strip(intext); // strip the ! from name

                if (isalpha(intext[0]) == 0){
                    printf("Constant name must start with Alpha character\n");
                    exit(1);
                }

                if (find(vars, intext) != -1){ // make sure the name doesnt exist
                    printf("Constant with name [ %s ] already declared!\n", intext);
                    exit(1);
                }

                float A = pop();

                nlink *temp_dict = dictionary(intext); // get a dictionary type link
                variable(temp_dict, A, CONSTANT); // assign it's value and set it type
                continue;
            }else if (intext[0] == '$'){ //variable creating
                if (DEBUG_MODE) printf("Creating a variable from top stack\n");
                float A = pop();
                strip(intext); //strip the $ from name

                if (isalpha(intext[0]) == 0){
                    printf("Variable name must start with Alpha character\n");
                    exit(1);
                }

                if (find(vars, intext) == -1){ // if name doesnt exist, create it
                    nlink *temp_dict = dictionary(intext); // get dictionary link
                    variable(temp_dict, A, VARIABLE);
                }else{
                    set(vars, intext, A); //else set its value
                }
                continue;
            }

            float A = 0;
            if (find(vars, intext) >= 0){ // check to see if name within dictionary
                int i = find(vars, intext);
                if (DEBUG_MODE) printf("[ %s ] at index %i\n", intext, i);

                put(get(vars, intext)); // get the variable value and put it into the stack
                continue;
            } else if (find(vars, intext) == -1 && strlen(intext) > 1){
                // if no name was found and input len is greater than 1 then raise error
                printf("NameError: Variable or Constant [ %s ] does not exist\n", intext);
                exit(1);
            } else {
                A = pop();
            }

            // if the input is only 1 charater see if its a operation
            for (int i = 0; i < 10; i++){
                if (i == 9){ // if i reaches 9 then it's not a operation character and something else
                    printf("NameError: Variable or Constant [ %s ] does not exist\n", intext);
                    exit(1);
                }
                if (operations_types[i] == intext[0]){
                    break;
                }
            }

            switch(intext[0]){  // look for operation. default exit program if nothing matches
                // preform the operation then put the value back onto the stack
                case '+':
                    if (DEBUG_MODE) printf("Here in plus\n");
                    put(A + pop());
                    break;
                case '-':
                    if (DEBUG_MODE) printf("Here in minus\n");
                    put(pop() - A);
                    break;
                case '*':
                    if (DEBUG_MODE) printf("Here in multiply\n");
                    put(A * pop());
                    break;
                case '/':
                    if (DEBUG_MODE) printf("Here in divide\n");
                    if (A == 0.0) { // check for A == 0.0 to prevent float by 0 exception. Exit if so
                        printf("Division by Zero Error!");
                        exit(1);
                    }
                    put(pop() / A);
                    break;
                case '^':
                    if (DEBUG_MODE) printf("Here in power\n");
                    put(pow(pop(), A));
                    break;
                case '&':
                    if (DEBUG_MODE) printf("Here in Dupilcate\n");
                    put(A);
                    put(A);
                    break;
                case '~':
                    if (DEBUG_MODE) printf("Here in Negative\n");
                    put(-A);
                    break;
                case '\'':
                    if (DEBUG_MODE) printf("Here in Invert\n");
                    if (A == 0.0) {
                        printf("Inversion Error! Division by Zero");
                        exit(1);
                    }
                    put(1.0 / A);
                    break;
                case '%':
                    if (DEBUG_MODE) printf("Here in Swap\n");
                    float B = pop();
                    put(A);
                    put(B);
                    break;
                default:
                    printf("Unknown Input!\n");
                    exit(1);
            }   // end switch

            if(PRINT_RESULTS) printf("output: %f\n", peek());
        }   // end else
	} // end while(true)

    if (DEBUG_MODE) printf("Printing Remain operands on Stack\n");
    DEBUG_MODE = 0;

    if (operands == NULL) { // if operands is NULL then nothings in the stack!
        printf("Empty Stack Error!\n");
    }else {
        while(operands != NULL){ // else loop over each element pop then print!
            printf("%f", pop());
            
            if (operands != NULL){
                printf("  ");    // if operands isnt NULL add two spaces
            }
        }
        printf("\n");
    }

	if(DEBUG_MODE) printf("Program Done");

	return 0;
} // end program
