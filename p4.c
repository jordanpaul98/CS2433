
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT   64
#define CONSTANT 2
#define VARIABLE 3
int DEBUG_MODE = 1;

// link list node(or link)
typedef struct link {
	float *value;
    int *type;
    char *name;
	struct link *head;
    struct link *tail;      
} nlink;

struct link *operands = NULL;   // initial link
struct link *vars = NULL; // intital variable space

nlink* dictionary(char *name){

    if (DEBUG_MODE) printf("Creating a link assoiated with name: [ %s ]\n", name);

    nlink* temp_link;
    temp_link = (nlink*)malloc(sizeof(nlink));
    if (temp_link == NULL) {                    // check make sure memory was allocated
        printf("Failed to allocate memory\n");
        exit(1);
    }

    temp_link->name = (char *)malloc(sizeof(name));
    if (temp_link->name == NULL){
        printf("Failed to allocate memory for dictionary name\n");
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
    
    strcpy(temp_link->name, name);

    return temp_link;
}

void variable(struct link *var, float value, int type){
    if (DEBUG_MODE) {
        printf("Assinging Variable [ %s ] to a ", var->name);
        if (type == CONSTANT){ printf("Constant with value: %f\n", value);
        }else{ printf("Variable with value: %f\n", value); }
    }
    *(var->value) = value;
    *(var->type) = type;

    var->head = vars;
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
    operands = temp_link;       // set operands to new link in chain

    if (DEBUG_MODE) printf("Added Value to Stack! %f\n", *(temp_link->value));
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

    if (DEBUG_MODE) printf("Popped Value from Stack!: %f\n", val);
    return val;  // return the float value popped from the head of the list
}

float get(nlink *link, char *name){
    struct link *loop_link = link;
    while (loop_link != NULL){
        if (loop_link->name == NULL || *loop_link->name == '\0'){
            printf("NameError: Link name contained a NULL name");
            exit(1);
        }

        if (strcmp(loop_link->name, name) == 0){
            return *(loop_link->value);
        }
        loop_link = loop_link->head;
    }
    printf("IndexError: [ %s ] not in list", name);
}

int index(nlink *link, char *name){
    struct link *loop_link = link;
    int _index = 0;
    while (loop_link != NULL){
        if (loop_link->name == NULL || *loop_link->name == '\0'){
            printf("NameError: Link name contained a NULL name");
            exit(1);
        }

        if (strcmp(loop_link->name, name) == 0){
            return _index;
        }

        _index ++;
        loop_link = loop_link->head;
    }
    return -1;
}

void printStack(){
    // prints the stack without modifying anything
    struct link *temp_operands = operands;
    if (temp_operands == NULL) { // if operands is NULL then nothings in the stack!
        printf("Nothing in Stack to Print!\n");
    }else {
        int count = 0;
        printf("Stack contents and index\n");
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
        printf("Stack contents and names\n");
        while(temp_vars != NULL){ // else loop over each element pop then print!
            if (temp_vars->type == CONSTANT) {
                printf("[CONSTANT] name [ %s ] %f\n", temp_vars->name, *(temp_vars->value));
            } else {
                printf("[VARIABLE] name [ %s ] %f\n", temp_vars->name, *(temp_vars->value));
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
        return; // Handle empty or NULL strings
    }

    int len = strlen(name);
    for (int i = 0; i < len - 1; i++) {
        name[i] = name[i + 1];
    }
    name[len - 1] = '\0';
}

void help(){
    printf("RNP Calculator Usage\n");
    printf("    #stop    - stop the program\n");
    printf("    #vars    - print the variable and constant stack\n");
    printf("    #show    - print the operands stack\n");
    printf("    ![name]  - pop top value of the stack and set it to a constant\n");
    printf("    $[name]  - pop top valye of the stack and set it to a variable\n");
    printf("    [name]   - get variable or constant by name\n");
    printf("    --debug  - enable program progression printing\n\n");
    printf("    Available Operators [+, -, *, /, ^, &, ~, \', %%]\n");
    printf("    +: Addition\n"); printf("    -: Substraction\n"); printf("    *: Multiplication\n"); printf("    /: Division\n");
    printf("    ^: Power\n"); printf("    &: Duplicate\n"); printf("    ~: Negate\n"); printf("    \': invert\n"); printf("    %%: swap\n\n");
}

int main(int argc, char* argv[]){

	float fpv;					// float input value
	char intext[MAX_INPUT];		// char array input value. set by max characters
    intext[MAX_INPUT - 1] = '\0';

	struct link *temp_link;     // temporary link pointer

    if (argc > 1) {
        if (strcmp(argv[1], "--debug") == 0){
            DEBUG_MODE = 1;
            printf("[PRGM] Debugging Enabled\n");
        }
    }

	while (1){                  // while loop until #stop encountered
		scanf("%s", intext);    // read next input or white space

        if (intext[MAX_INPUT - 1] != '\0'){ //make sure the input string is valid
            printf("Maximum string input reached: Current limit- %i characters", MAX_INPUT);
            exit(1);
        } else if (strcmp(intext, "#stop") == 0){ // nothing else to do. Break!
            break;
        } else if (strcmp(intext, "--help") == 0){
            help();
            continue;
        } else  if (strcmp(intext, "#show") == 0){
            printStack();
            continue;
        } else if (strcmp(intext, "#vars") == 0){
            printVariables();
            continue;
        } else if (strcmp(intext, "--debug") == 0){
            if (DEBUG_MODE){
                DEBUG_MODE = 0;
                printf("[PRGM] Debugging Disabled\n");
            }else{
                DEBUG_MODE = 1;
                printf("[PRGM] Debugging Enabled\n");
            }
            continue; 
        }

		if (sscanf(intext, "%f", &fpv) == 1){ // if value value was read, put it into the operands stack
			put(fpv);
		}else { // else preform a operation

            if (intext[0] == '!'){
                if (DEBUG_MODE) printf("Creating a constant from top stack\n");
                float A = pop();
                strip(intext);

                if (index(vars, intext) != -1){
                    printf("Constant with Name already declared!");
                    exit(1);
                }

                nlink *temp_dict = dictionary(intext);
                variable(temp_dict, A, CONSTANT);
                continue;
            }else if (intext[0] == '$'){
                if (DEBUG_MODE) printf("Creating a variable from top stack\n");
                float A = pop();
                strip(intext);

                if (index(vars, intext) == -1){
                    nlink *temp_dict = dictionary(intext);
                    variable(temp_dict, A, VARIABLE);
                }else{
                    printf("Need to reset variable");
                }
                continue;
            }

            float A = 0;
            if (strlen(intext) == 1){
                A = pop();
            } else{
                int i = index(vars, intext);
                if (DEBUG_MODE) printf("[ %s ] at index %i\n", intext, i);
                continue;
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
                    printf("Wrong Operation Entry!\n");
                    exit(1);
            }   // end switch
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
