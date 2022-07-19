#include <stdio.h>
#include <stdlib.h>

// if the condition is true, print an error message and terminates the program
// @param condition: boolean condition to exit the program
// @param message: message to be printed on the console
void crash_program_if(int condition, char* message)
{
    if(condition) { printf("ERROR: %s\nExiting...\n", message); exit(EXIT_FAILURE); }
}

// print an error message on the console
void print_error(char* message) 
{
    printf("ERROR: %s\n", message);
}