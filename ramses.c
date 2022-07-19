#include <stdio.h>      // printf(), sizeof(), fopen(), fclose(), fread(), FILE, fprintf()
#include <stdlib.h>     // malloc(), free(), exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>     // memcmp()

#include "ramses.h"

// must start with this 4 bytes
const char RAMSES_SIGNATURE[SIGNATURE_SIZE] = {0x03, 0x52, 0x4d, 0x53};

// tries to open a ramses .mem file and saves in memory, or else return NULL
// @param filename: name of the ramses file to be open, a .mem file
RAMSES_FILE* open_ramses_file(char* filename)
{
    FILE* file = fopen(filename, "rb");
    RAMSES_FILE* ramses_file = (RAMSES_FILE*)malloc(sizeof(RAMSES_FILE));

    // openning file error
    if(!file) { 
        if(ramses_file) free(ramses_file);
        return NULL; 
    }
    // allocation error
    if(!ramses_file) { fclose(file); return NULL; }

    // tries to read from the opening file exactly the size of a ramses file once
    // if unnable to read the exaclty size, then we have a
    // reading file error
    if( fread(ramses_file, sizeof(RAMSES_FILE), 1, file) != 1) {
        free(ramses_file);
        ramses_file = NULL;
    }

    fclose(file);
    return ramses_file;
}

// returns true if the suposed ramses file has the signature of a ramses file
// @param suposed_ramses_file: an opened ramses file using RAMSES_FILE structure
int is_ramses_file(RAMSES_FILE* suposed_ramses_file) 
{
    return (memcmp(RAMSES_SIGNATURE, suposed_ramses_file->signature, SIGNATURE_SIZE) == 0);
}

// prints the hexadecimal signature of the ramses file
// @param ramses_file: an opened ramses file using RAMSES_FILE structure
void print_signature(RAMSES_FILE* ramses_file) 
{
    for(int i = 0; i < SIGNATURE_SIZE; ++i) 
        printf("%02X ", ramses_file->signature[i]);
    printf("\n");
} 

// prints the ramses program code in hexadecimal, in 'columns' per line
// @param ramses_file: an opened ramses file using RAMSES_FILE structure
// @param columns: defines how many columns to print the memory data
// @param stream: the file stream where the information will be printed
void print_program(RAMSES_FILE* ramses_file, const unsigned int columns, FILE* stream)
{
    if(!columns) {
        printf("ERROR: (at print_program()) \'columns\' must be greater then 0!\n");
        return;
    }

    fprintf(stream, "%02X: ", 0);
    for(int address = 0; address < PROGRAM_SIZE; ++address) {
        // prints the address on the next line when column max range reached
        if(address && address % columns == 0) 
            fprintf(stream, "\n%02X: ", address);
        // prints the program data of the address given
        fprintf(stream, "%02X ", (unsigned char)(ramses_file->program[address][0]));
    }
    fprintf(stream, "\n");
}

// saves a ramses file to disk
// @param new_filename: the new name for the ramses file
// @param ramses_file: an opened ramses file using RAMSES_FILE structure
int save_ramses_file(char* new_filename, RAMSES_FILE* ramses_file) 
{
    FILE* save_ramses = fopen(new_filename, "wb");
    if(!save_ramses) return 0;

    int r = 1;

    if( fwrite(ramses_file, sizeof(RAMSES_FILE), 1, save_ramses) != 1 ) 
        r = 0;

    fclose(save_ramses);
    return r;
}