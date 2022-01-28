#ifndef PERCENTLIB_H
#define PERCENTLIB_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// get the length and subtract 8 (4 for the number and 4 for [] %)
#define OVERHEAD_LEN 8

char * fillBar(char * str, long len, float pnum);


#endif  // PERCENTLIB_H
