#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define DEFLEN 15

int fillBar(char * str, int len, int pnum);

int main(int argc, char ** argv){
    // TODO finish my parser so i can use it here
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    // get the length and subtract 6 (3 for the number and 3 for []%)
    int len = w.ws_col - 6;
    // check to make sure the len is good (am i in a terminal, if not then checking col is meaningless)
    if(len <= 0 || !isatty(fileno(stdin))){
        len = DEFLEN;
    }
    int pnum = 0;
    if(argc == 3){
        len = atoi(argv[1]);
        pnum = atoi(argv[2]);
    }else if((argc > 3) || (argc < 2)){
        return argc;
    }else{
        pnum = atoi(argv[1]);
    }
    char * str = (char *) malloc(sizeof(char) * (len+7));
    /* const struct timespec rqtp = {0, 100000000}; */
    /* for(int i = 0; i < 100; i++){ */
    /*     fillBar(str, len, i); */
    /*     printf("\r%s", str); */
    /*     fflush(stdout); */
    /*     nanosleep(&rqtp, NULL); */
    /* } */
    /* for(int i = 99; i > 0; i--){ */
    /*     fillBar(str, len, i); */
    /*     printf("\r%s", str); */
    /*     fflush(stdout); */
    /*     nanosleep(&rqtp, NULL); */
    /* } */
    /* printf("\n"); */
    fillBar(str, len, pnum);
    printf("%s", str);


    free(str);
    return 0;
}

int fillBar(char * str, int len, int pnum){
    int num2 = ((pnum > 100) ? 100 : pnum) * 2 * len / 100;
    int num = num2 / 2;
    str[0] = '[';
    str[len+1] = ']';
    memset(str+1, (int) '=', num);
    if(num2 % 2)    str[++num] = '~';
    memset(str+1+num, (int) '-', len-num);
    sprintf(str+len+2, "%3d%%\0", pnum);
    // TODO maybe just clamp pnum at 100 before entering the function
    return pnum > 100;
}
