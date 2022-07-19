#ifndef __GENERATE_H__
#define __GENERATE_H__

#define MAX_NODES 15
#define ADDRESS_START ((unsigned char)0x83) // 3 bytes upcoming existent node at 0x80
#define ADDRESS_STOP ((unsigned char)0xED)  // 2 bytes prior to address 0xEF (end address)
#define MAX_ADDRESSES_AVAILABLE (ADDRESS_STOP - ADDRESS_START) 
#define MAX_RANDOM_CALLS (MAX_ADDRESSES_AVAILABLE * 2)
#define ADDRESS_NOT_FOUND 0
#define ADDRESS_INIT ((unsigned char)0x80)
#define ADDRESS_END ((unsigned char)0xEF)
#define MAX_DASHES 75

typedef struct NODE {
    char value;
    unsigned char address_left;
    unsigned char address_right;
} NODE;

typedef struct NODE_INFO {
    NODE node;
    unsigned char address;
} NODE_INFO; 

// print a dash line in a stream file
// @param fileout: output stream to print content
// @param max_dashes: number of dashes to print
void fprint_line(FILE* fileout, int max_dashes);

// returns a ramses file with ramdomly assigned nodes at specified addresses
// @param ramses_file: ramses file to randomly add nodes and update
// @param output_info: stream to output the info generated
RAMSES_FILE* create_updated_ramses_file(RAMSES_FILE* ramses_file, FILE* output_info);

#endif//__GENERATE_H__
