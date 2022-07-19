#ifndef __RAMSES_H__
#define __RAMSES_H__

#define SIGNATURE_SIZE 4
#define PROGRAM_SIZE 256
#define PROGRAM_COLUMNS 8

/*
    Notice that a ramses memory file has 516 bytes size
    The first 4 bytes are the signature
    The ramses program is distributed between 512 bytes 
    And the instructions has a 2 byte structure, little endian like format 
    But only the first byte is ever used, 
    meaning that the second byte is a don't care'ish with a zero value 
        Example: program that sets a variable
            ORG h00
            var: DB 1
            HLT
        Compiles to: 
            | 03 52 4d 53 | -> signature
            | 01 00 | f0 00 | -> var and HLT, respectively
            ...[508 zeroed bytes]
*/
typedef struct RAMSES_FILE {
    char signature[SIGNATURE_SIZE];
    char program[PROGRAM_SIZE][2];
} RAMSES_FILE; 

// for extern use if needed
// extern const char RAMSES_SIGNATURE[SIGNATURE_SIZE];
// extern const int RAMSES_SIZE; // tamanho do arquivo .mem do ramses

// tries to open a ramses .mem file and saves in memory, or else return NULL
// @param filename: name of the ramses file to be open, a .mem file
RAMSES_FILE* open_ramses_file(char* filename);

// returns true if the suposed ramses file has the signature of a ramses file
// @param suposed_ramses_file: an opened ramses file using RAMSES_FILE structure
int is_ramses_file(RAMSES_FILE* suposed_ramses_file);

// prints the hexadecimal signature of the ramses file
// @param ramses_file: an opened ramses file using RAMSES_FILE structure
void print_signature(RAMSES_FILE* ramses_file);

// prints the ramses program code in hexadecimal, in 'columns' per line
// @param ramses_file: an opened ramses file using RAMSES_FILE structure
// @param columns: defines how many columns to print the memory data
// @param stream: the file stream where the information will be printed
void print_program(RAMSES_FILE* ramses_file, const unsigned int columns, FILE* stream);

// saves a ramses file to disk
// @param new_filename: the new name for the ramses file
// @param ramses_file: an opened ramses file using RAMSES_FILE structure
int save_ramses_file(char* new_filename, RAMSES_FILE* ramses_file);

#endif//__RAMSES_H__