#include <stdio.h>      // fprintf(), FILE, printf(), sizeof()
#include <stdlib.h>     // calloc, free(), malloc()
#include <string.h>     // memcpy()

#include "ramses.h"     // RAMSES_FILE
#include "random.h"     // random_between()
#include "generate.h"   

// returns a random number between 0 ~ 255 (included)
// used to get a random value for the node 
char get_random_value(void) { return (char)random_between(0, 255); }

// returns a random number between ADDRESS_START ~ ADDRESS_STOP (included)
// used to get a random address for the node
unsigned char get_random_address(void)
{
    return (unsigned char)random_between(ADDRESS_START, ADDRESS_STOP);
}

// returns a random number between 0 ~ MAX_NODES (included)
// used to get a random number to define how many nodes will be created
int get_random_max_nodes(void) { return random_between(1, MAX_NODES); }

// randomly choose values for the nodes
// @param nodes: static array of NODE_INFO of size MAX_NODES
// @param number_of_nodes: number of nodes (from 0 to number_of_nodes) to set the node.value
void set_nodes_value(NODE_INFO nodes[MAX_NODES], int number_of_nodes)
{
    for(int i = 0; i < number_of_nodes; ++i)
        nodes[i].node.value = get_random_value();
}

unsigned char map_address_to_array(unsigned char address) { return address - ADDRESS_START; }

// returns true if the given address and 2 bytes next are occupied, otherwise returns false 
// @param addresses_occupied: dinamically array that of size that maps to the addresses used
// @param address: address to map to the array and verify if the position is available
int is_not_available_address(unsigned char* occupied_addresses, unsigned char address)
{
    return occupied_addresses[address] || occupied_addresses[address+1] || occupied_addresses[address+2];
}

// sets the given address and the 2 bytes next to occupied 
// @param addresses_occupied: dinamically array of size that maps to the addresses used
// @param address: address to map to the array and set the position to unavailable
void set_address_occupied(unsigned char* occupied_addresses, unsigned char address)
{
    occupied_addresses[address] = occupied_addresses[address+1] = occupied_addresses[address+2] = 1;
}

// searches for 3 free bytes contiguously in memory and if found returns its start address
// @param addresses_occupied: dinamically array of size that maps to the addresses used
unsigned char search_available_address(unsigned char* occupied_addresses)
{
    int limit = MAX_ADDRESSES_AVAILABLE - 2;

    for(int mem_pos = 0; mem_pos < limit; ++mem_pos) {
        if( !is_not_available_address(occupied_addresses, mem_pos) ) 
            return mem_pos + ADDRESS_START;
    }

    return ADDRESS_NOT_FOUND;
}

// randomly choose addresses for the nodes, returns 1 if successful or 0 otherwise
// @param nodes: static array of NODE_INFO of size MAX_NODES
// @param number_of_nodes: number of nodes (from 0 to number_of_nodes) to set the address
int set_nodes_address(NODE_INFO nodes[MAX_NODES], int* number_of_nodes)
{
    // creates an array with the size of available spaces for the nodes
    // it will be used to verify if address randomly choosed is already being used for other node
    unsigned char* occupied_addresses = (unsigned char*)calloc(MAX_ADDRESSES_AVAILABLE, sizeof(unsigned char));
    if(!occupied_addresses) return 0;

    // sets the first node address
    nodes[0].address = ADDRESS_INIT;
    
    // loop starts with the second node
    for(int nodes_count = 1; nodes_count < *number_of_nodes; ++nodes_count) {
        unsigned char current_address = 0;
        int count_max_random_calls = 0;

        // gets a random address if available or keep trying 
        // until find one available or until loop limit is reached
        do {
            current_address = get_random_address();
            ++count_max_random_calls;
        } while( 
            is_not_available_address(occupied_addresses, map_address_to_array(current_address)) && 
            count_max_random_calls <= MAX_RANDOM_CALLS
        );

        // if no random address is found tries to search for one
        if(count_max_random_calls == MAX_RANDOM_CALLS) {
            unsigned char available_address = search_available_address(occupied_addresses);

            // in this case, there is no more space to place one node
            // then continuing is useless, we update the pointer to number_of_nodes
            // so the number can represent the real quantity of nodes 
            if(available_address == ADDRESS_NOT_FOUND) {
                *number_of_nodes = nodes_count;
                break;
            } 
            
            nodes[nodes_count].address = available_address;
            set_address_occupied(occupied_addresses, map_address_to_array(available_address));
        } 
        // an random address is found, proceed normally 
        else {
            nodes[nodes_count].address = current_address; 
            set_address_occupied(occupied_addresses, map_address_to_array(current_address));
        }
    }

    free(occupied_addresses);
    return 1;
} 

// connects all node addresses from the first node until the last node
// @param nodes: static array of NODE_INFO of size MAX_NODES
// @param number_of_nodes: number of nodes (from 0 to number_of_nodes) to make the tree connections
void set_nodes_connections(NODE_INFO nodes[MAX_NODES], int number_of_nodes) 
{
    int limit = number_of_nodes - 1;

    for(int nodes_count = 0; nodes_count < limit; ++nodes_count) {
        // connect the left pointer
        if(nodes[nodes_count].node.value < 0) {
            nodes[nodes_count].node.address_left = nodes[nodes_count+1].address;
            nodes[nodes_count].node.address_right = (unsigned char)get_random_value();
        } 
        // connect the right pointer
        else {
            nodes[nodes_count].node.address_left = (unsigned char)get_random_value();
            nodes[nodes_count].node.address_right = nodes[nodes_count+1].address;
        }
    }

    // the last node must point to null (0)
    if(nodes[limit].node.value < 0) {
         nodes[limit].node.address_left = 0;
         nodes[limit].node.address_right = (unsigned char)get_random_value();
    } 
    else {
        nodes[limit].node.address_left = (unsigned char)get_random_value();
        nodes[limit].node.address_right = 0;
    }
}

// in a ramses file put the nodes in their addresses
// @param ramses_file: ramses file to copy the nodes to
// @param nodes: static array of NODE_INFO of size MAX_NODES
// @param number_of_nodes: number of nodes (from 0 to number_of_nodes) to copy to the ramses file
void update_ramses_file(RAMSES_FILE* ramses_file, NODE_INFO nodes[MAX_NODES], int number_of_nodes) 
{
    for(int nodes_count = 0; nodes_count < number_of_nodes; ++nodes_count) {
        ramses_file->program[ nodes[nodes_count].address ][0] = nodes[nodes_count].node.value;
        ramses_file->program[ nodes[nodes_count].address+1 ][0] = nodes[nodes_count].node.address_left;
        ramses_file->program[ nodes[nodes_count].address+2 ][0] = nodes[nodes_count].node.address_right;
    }
} 

// print a dash line in a stream file
// @param fileout: output stream to print content
// @param max_dashes: number of dashes to print
void fprint_line(FILE* fileout, int max_dashes) 
{ 
    for(int i = 0; i < max_dashes; ++i) 
        fprintf(fileout, "-"); 
    fprintf(fileout, "\n"); 
}

// print information about quantity, nodes and nodes connection to a stream file
// @param fileout: output stream to print content
// @param nodes: static array of NODE_INFO of size MAX_NODES
// @param number_of_nodes: number of nodes (from 0 to number_of_nodes) to print to stream file
void fprint_info(FILE* fileout, NODE_INFO nodes[MAX_NODES], int number_of_nodes)
{
    fprintf(fileout, "Number of nodes: %02d\n", number_of_nodes);
    fprint_line(fileout, MAX_DASHES);
    
    // printf node values and address location
    for(int nodes_count = 0; nodes_count < number_of_nodes; ++nodes_count) {
        fprintf(fileout, "Node at address %02X\n", nodes[nodes_count].address);
        fprintf(fileout, "Value: %02X (%d)\tLeft Address: %02X\tRight Address: %02X\n", 
            (unsigned char)nodes[nodes_count].node.value, 
            nodes[nodes_count].node.value, 
            nodes[nodes_count].node.address_left, 
            nodes[nodes_count].node.address_right
        );
        fprint_line(fileout, MAX_DASHES);
    }
    fprintf(fileout, "\n");

    // printf the suposed ramses program output
    fprintf(fileout, "OUTPUT: %02d nodes [", number_of_nodes);
    for(int nodes_count = 0; nodes_count < number_of_nodes; ++nodes_count) {
        fprintf(
            fileout, 
            (nodes_count == number_of_nodes-1) ? "%02X(%d)" : "%02X(%d), ", 
            (unsigned char)nodes[nodes_count].node.value,
            nodes[nodes_count].node.value
        );
    }
    fprintf(fileout, "]\n\n");
}

// returns a ramses file with ramdomly assigned nodes at specified addresses
// @param ramses_file: ramses file to randomly add nodes and update
// @param output_info: stream to output the info generated
RAMSES_FILE* create_updated_ramses_file(RAMSES_FILE* ramses_file, FILE* output_info) 
{
    // notice that for simplicity we are statically allocating space for 15 nodes, but we might use less
    NODE_INFO nodes[MAX_NODES] = {0};

    // gets an random number that will be used to set how many nodes we'll use
    int number_of_nodes = get_random_max_nodes(); 

    // allocate memory to a new ramses file
    RAMSES_FILE* new_ramses_file = (RAMSES_FILE*)malloc(sizeof(RAMSES_FILE));
    if(!new_ramses_file) return NULL;

    // copies the content to the copy from the original
    memcpy(new_ramses_file, ramses_file, sizeof(RAMSES_FILE));

    // randomly assign the values for each node
    set_nodes_value(nodes, number_of_nodes); 

    // tries to randomly assign the addresses position for each node
    if(!set_nodes_address(nodes, &number_of_nodes)) {
        free(new_ramses_file);
        printf("ERROR: Unnable to set the node addresses!\n");
        return NULL;
    } 

    // "connect" the nodes with each other (tree structure like connection)
    set_nodes_connections(nodes, number_of_nodes); 

    // put the nodes on the memory of the new ramses file
    update_ramses_file(new_ramses_file, nodes, number_of_nodes); 

    // print on the output file information about this nodes and the connections
    fprint_info(output_info, nodes, number_of_nodes); 

    return new_ramses_file;
} 

