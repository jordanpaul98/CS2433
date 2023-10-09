/*
Program 3: stdin, Reading and Processing RPN
CS 2433 Fall 23
Richard L. Churchill
Due: Sep 23, 2023
Completed: 09/21/2023
Jordan Paul, jopaul

read input from stdin until #stop input is encountered. Then prints the results
of the operations indicated from stdin

operands (floating point values)
operators (+, -, *, /)

if floating point is read, add the value to the operand stack
if a operator is read, perform the operation with two top values in the stack
then push the computed value to the stack

pop the stack to get value A, pop stack again to get value B. if there is not element to
pop from the stack raise. "Stack Underflow Error!\n" and exit
(+) A + B
(-) B - A
(*) A * B
(/) B / A    #note if A = 0.0 then divsion by zero error is raised and program will exit
inputing any other values besides a float or (+, -, *, /) will cause the program to exit

when #stop is enounter, print the elements of the stack. if there is not elements to
print then raise. "Empty Stack Error!"

handin code:
handin cs2433-rlchurc program3 p3.c

USAGE:

10 10 + 5 -         NOTE: same as 10 + 10 - 5
#stop
15

10 5 * 20 - 6 /
#stop               NOTE: same as ((5 * 10) - 20)/6
5

15 15 20 -
-
#stop               NOTE: same as 15 - (15 - 20)
20

*/

// standard library for printing, reading input, and comparing strings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// maximum char array to read from stdin
#define MAX_INPUT   64

// link list node(or link)
typedef struct link {
	float *value;           // float value
	struct link *next;      // prvious link in the list
} nlink;

struct link *operands = NULL;   // initial link

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
    
    
    temp_link->next = operands; // set temp link to point to previous link
    operands = temp_link;       // set operands to new link in chain

    //printf("Added Value to Stack! %f\n", *(temp_link->value));
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
    struct link *stack = operands->next; // set a temp link pointing to next link in the list

    freeLink(operands); // free the head of the link list

    operands = stack; // set the operands(head) to temp link

    //printf("Popped Value from Stack!: %f\n", val);
    return val;  // return the float value popped from the head of the list
}

int main(int argc, char* argv[]){

	float fpv;					// float input value
	char intext[MAX_INPUT];		// char array input value. set by max characters
    intext[MAX_INPUT - 1] = '\0';

	struct link *temp_link;     // temporary link pointer

	while (1){                  // while loop until #stop encountered
		scanf("%s", intext);    // read next input or white space

        if (intext[MAX_INPUT - 1] != '\0'){ //make sure the input string is valid
            printf("Maximum string input reached: Current limit- %i characters", MAX_INPUT);
            exit(1);
        }

		if (strcmp(intext, "#stop") == 0){ // nothing else to do. Break!
            break;
        }

		if (sscanf(intext, "%f", &fpv) == 1){ // if value value was read, put it into the operands stack
			put(fpv);
		}else { // else preform a operation
            float A = pop();    // pop top stack to get value A
            float B = pop();    // pop stack again to get Value B

            switch(intext[0]){  // look for operation. default exit program if nothing matches
                // preform the operation then put the value back onto the stack
                case '+':
                    // printf("Here in plus\n");
                    put(A + B);
                    break;
                case '-':
                    // printf("Here in minus\n");
                    put(B - A);
                    break;
                case '*':
                    // printf("Here in multiply\n");
                    put(A * B);
                    break;
                case '/':
                    // printf("Here in divide\n");
                    if (A == 0.0) { // check for A == 0.0 to prevent float by 0 exception. Exit if so
                        printf("Division by Zero Error!");
                        exit(1);
                    }
                    put(B / A);
                    break;
                default:
                    printf("Wrong Operation Entry!\n");
                    exit(1);
            }   // end switch
        }   // end else
	} // end while(true)

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

	// printf("Program Done");

	return 0;
} // end program
