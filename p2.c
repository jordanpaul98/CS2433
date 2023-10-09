/*
jordan paul Fall 2023

	Write a program in C, named p2.c, which accepts input from stdin,
	and creates a linked list of distinct inputs until a ‘#stop’ input is encountered,
	then prints the data elements of the linked list in two passes,
	the first pass printing the floating point values read and the second pass printing the strings truncated to seven printable characters

	input must distinguish between floating point and strings

	descriminators need to be used to distinguish between float and string

	input of "#stop" will stop inputs from being accepted and print out the contents of link list

	all floats will be printed first

	rest of strings will be printed second


    Usage:

	stdin: 		>hello
				>there
				>123
				>45.6
				>.789
				>done
				>#stop

   	outputs:	0.789000
   				45.600000
   				123.000000
   				done
   				there
   				hello
*/


// standard library for printing, reading input, and comparing strings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// desriminators for distinguishing between floating point and string
#define DISCRIM_FLT 1
#define DISCRIM_STR 2

// define maximum input and maxinimum string to store
#define STR_LEGNTH  8
#define MAX_INPUT   128

// contents of the link list
union content {
	float fv;				// floating point value
	char  tx[STR_LEGNTH];	// char array with defined max length
};

typedef struct link {
	int discrim;			// float or string idenifier
	union content c;		// float or string value
	struct link *next;		// next pointer to the previous link. Looking towards tail.
} nlink;

// create null link to start list
struct link *head = NULL;

int main(int argc, char* argv[]){

	float fpv;					// float input value
	char intext[MAX_INPUT];		// char array input value. set by max characters

	struct link *tlink;			// temporary link pointer

	while (1){
		scanf("%s", intext);    //read input from user enter commands

		if (strcmp(intext, "#stop") == 0) break;  // if text string is #stop - exit

		//printf("printing: %s\n", intext)

		tlink = malloc(sizeof(tlink));  // if text is not stop. allocate memory for new link

		if (tlink == NULL){	// if tlink is NULL then exit as memory wasn't allocated correctly
			printf("Failed to Allocate memory");
			exit(1);
		}

		// check see if the input string was a floating point value and assign fpv to it
		if (sscanf(intext, "%f", &fpv) == 1){
			tlink->c.fv = fpv;              	// assign temp link to float value
			tlink->discrim = DISCRIM_FLT;		// set the distinguisher to float
		}else{
			// else it was a string. copy contents up to string length
			strncpy(tlink->c.tx, intext, STR_LEGNTH - 1);
			tlink->c.tx[STR_LEGNTH-1] = '\0';  	// manually set last element of char array to null to end line
			tlink->discrim = DISCRIM_STR;		// set the distingusher to string
		}

		tlink->next = head; // set the temp list next to global head link list
		head = tlink;		// set the global head link list to temp link list

		// continue onto next user input
	}

	// create a temporary link list used for looping through contents and set it to global head list
	struct link *tHead = head;

	// loop through each link list until null list is encountered
	while (tHead != NULL){
			if (tHead->discrim == DISCRIM_FLT){ // if link is a float. print it to newline
				//printf("float:  %f\n", tHead->c.fv);
				printf("%f\n", tHead->c.fv);
			}
			tHead = tHead->next; // set temp link to next link
		}

	tHead = head; // point temp back to the head as it has looped through whole list
	// loop through each link list until null list is encountered
	while (tHead != NULL){
		if (tHead->discrim == DISCRIM_STR){ // if link is a string. print it to newline
			//printf("String:  %s\n", tHead->c.tx);
			printf("%s\n", tHead->c.tx); // if link is a string. print it to newline
		}
		tHead = tHead->next; // set temp link to next link
	}


	// printf("Program Done");

	return 0;
} // end program

