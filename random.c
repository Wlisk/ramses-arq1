#include <time.h>
#include <stdlib.h>

// intiliaze the random seed
void initilize_random(void) 
{
    srand( (unsigned int)time(NULL) );
} 

// get an integer between min and max (included)
int random_between(int min, int max) 
{
    return (rand() % (max - min + 1)) + min;  
}