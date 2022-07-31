#include <stdio.h>      // printf(), fopen(), fclose()
#include <stdlib.h>     // free(), EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>     // strcmp()

#include "ramses.h"
#include "error.h"
#include "random.h"     // initilize_random()
#include "generate.h"   // create_updated_ramses_file()

#define ARG_FILENAME 1 

// returns the value (string) for the option given from the program arguments
char* get_argv_option_value(int argc, char* argv[], char* option);

// returns true if the options (flag) is setted om the program arguments, or false otherwise
int is_argv_option_setted(int argc, char* argv[], char* option);

// prints help information to the console
void print_help(void);

int main(int argc, char* argv[])
{
    RAMSES_FILE* ramses_file = NULL;

    // no file name passed as argument
    // expecting a ramses memory file (ex.: program.mem)
    crash_program_if(argc < 2, "Must pass the ramses mememory file name as first argument!");

    if(is_argv_option_setted(argc, argv, "--help") || is_argv_option_setted(argc, argv, "-h")) {
        print_help();
        return EXIT_SUCCESS;
    }

    ramses_file = open_ramses_file(argv[ARG_FILENAME]);
    crash_program_if(!ramses_file, "Unnable to read the ramses .mem file!");

    // doesn't have the signature of a ramses file
    if(!is_ramses_file(ramses_file)) {
        print_signature(ramses_file);
        printf("ERROR: %s doesn't have a signature of ramses .mem file\n", argv[ARG_FILENAME]);

        free(ramses_file);
        return EXIT_FAILURE;
    }

    // print the ramses file memory if argument is setted and exit
    if(is_argv_option_setted(argc, argv, "--print") || is_argv_option_setted(argc, argv, "-p")) {
        print_program(ramses_file, PROGRAM_COLUMNS, stdout);
        free(ramses_file);
        return EXIT_SUCCESS;
    }

    // configure the seed for random numbers
    initilize_random();

    // buffers to dinamically alter strings of the output filename that'll be saved in disk memory
    char filename_buffer[50] = {0};
    char filename_info_buffer[60] = {0};

    // number of files to be created
    int max_creations = 1;

    // gets the number of files to be created from program argument if setted
    char* number = get_argv_option_value(argc, argv, "-n");
    if(number) {
        max_creations = atoi(number);
        if(!max_creations) max_creations = 1;
    }

    for(int i = 0; i < max_creations; ++i) { 
        // dinamically alter the output filenames
        sprintf(filename_buffer, "new_%02d.mem", i);
        sprintf(filename_info_buffer, "new_%02d.info.txt", i);

        // tries to create the output for information of the current ramses file that'll be created
        FILE* info_output = stdout;

        // if argument --stdout is passed, print output information on stdout
        // otherwise use default method
        if(!( is_argv_option_setted(argc, argv, "--stdout") )) {
            info_output = fopen(filename_info_buffer, "w");
            if(!info_output) { 
                printf("ERROR: Unnable to create file information for %s, using stdout...\n", 
                    filename_buffer
                );
                info_output = stdout;
            }
        }

        // for better console visualisation only
        if(info_output == stdout) {
            fprint_line(stdout, MAX_DASHES);
            fprint_line(stdout, MAX_DASHES);
            printf("\nNODE %d:\n", i);
        }

        int daedalus = 0;
        if( 
            is_argv_option_setted(argc, argv, "--daedalus") || 
            is_argv_option_setted(argc, argv, "-d")
        )
            daedalus = 1;
        
        RAMSES_FILE* new_ramses_file = create_updated_ramses_file(ramses_file, info_output, daedalus);

        if(new_ramses_file) {
                print_program(new_ramses_file, PROGRAM_COLUMNS, info_output);

                if(!daedalus)
                    if( !save_ramses_file(filename_buffer, new_ramses_file) ) 
                        printf("ERROR: Unnable to save the %s file!\n", filename_buffer);

                free(new_ramses_file);
        } else printf("ERROR: Unnable to create and set %s file!\n", filename_buffer);
        printf("\n");

        if(info_output != stdout) fclose(info_output);
    }
    printf("DONE!\n");

    free(ramses_file);
    return EXIT_SUCCESS;
} 

// returns the value (string) for the option given from the program arguments
char* get_argv_option_value(int argc, char* argv[], char* option)
{
    int limit = argc - 1;
    for(int i = 0; i < limit; ++i) {
        if( strcmp(argv[i], option) == 0 ) return argv[i+1];
    }
    return NULL;
}

// returns true if the options (flag) is setted om the program arguments, or false otherwise
int is_argv_option_setted(int argc, char* argv[], char* option) 
{
    for(int i = 0; i < argc; ++i) {
        if( strcmp(argv[i], option) == 0 ) return 1;
    }
    return 0;
}

// prints help information to the console
void print_help(void) 
{
    printf("Program to create a tree like structure with random values and addresses\nto use and put in memory of a RAMSES computer architecture. \nProject created to help with college work from UFRGS, Computer Architecture I\n\n");
    printf("Usage: <program> <ramses-memory-file>.mem  <..options>\n");
    printf("- This program will create a new <ramses-memory-file>.mem (or as much as needed)\n");
    printf("- The new <ramses-memory-file>.mem will be named as new_<number>.mem\n");
    printf("- Information about how many nodes was created and their connections\ncan be seen at new_<number>.info.txt or at stdout if flag is setted\n\n");
    fprint_line(stdout, MAX_DASHES);

    printf("Options:\n");
    printf("\t--help, -h\t> Print this message.\n");
    printf("\t--print, -p\t> Prints the memory from <ramses-memory-file>.mem and \n\t\t\texit program.\n");
    printf("\t-n <number>\t> Number of new <ramses-memory-file>.mem to be created, \n\t\t\teach randomly unique. 1 is the default value.\n");
    printf("\t--stdout\t> Prints information about the new <ramses-memory-file> \n\t\t\tcreated into the stdout instead of new files that would \n\t\t\tbe saved into disk.\n");
    printf("\t--daedalus, -d\t> If setted, prints information about nodes in the Daedalus assembler format and don't generate the new .mem files\n");
}