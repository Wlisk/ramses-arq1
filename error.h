#ifndef __ERROR_H__
#define __ERROR_H__

// if the condition is true, print an error message and terminates the program
// @param condition: boolean condition to exit the program
// @param message: message to be printed on the console
void crash_program_if(int condition, char* message);

// print an error message on the console
void print_error(char* message);

#endif//__ERROR_H__