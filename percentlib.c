#include "percentlib.h"

char * fillBar(char * str, long len, float pnum){
    // allocate an array if given NULL
    if(str == NULL){
        str = (char *) calloc(len+1, sizeof (char));
    }
    float num = pnum * (len - OVERHEAD_LEN) / 100;
    str[0] = '[';
    str[len-(OVERHEAD_LEN-1)] = ']';
    memset(str+1, '=', (int)num);
    if((int)(num * 2) % 2)   str[(int)++num] = '~';
    memset(str+1+(int)num, '-', len-OVERHEAD_LEN-(int)num);
    snprintf(str+len-6, 6+1, " %#4.3g%%", pnum);
    // add a percent if it got cut off cuase the %g went too big (NOTE this doesnt round so it will be wrong)
    str[len-1] = '%';
    return str;
}
