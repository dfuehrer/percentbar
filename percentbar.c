#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include "percentlib.h"

#define DEFLEN 15

int main(int argc, char ** argv){
    // TODO finish my parser so i can use it here
    long len = 0;
    // check to make sure the len is good (am i in a terminal, if not then checking col is meaningless)
    if(len <= 0 || !isatty(fileno(stdin))){
        len = DEFLEN;
    }
    float pnum = 0;
    if((argc > 3) || (argc < 2)){
        fprintf(stderr, "Usage:\t%s [width] percent\n", argv[0]);
        return argc;
    }
    errno = 0;
    pnum = strtof(argv[argc-1], NULL);
    if(errno){
        err(errno, "Can't parse percent '%s' as a float...", argv[2]);
    }
    if(pnum > 100){
        pnum = 100;
    }else if(pnum < 0){
        pnum = 0;
    }
    if(argc == 3){
        errno = 0;
        len = strtol(argv[1], NULL, 0);
        if(errno){
            err(errno, "Can't parse width '%s' as a int...", argv[1]);
        }
    }else{
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        len = w.ws_col;
    }
    if(len <= OVERHEAD_LEN){
        err(EXIT_FAILURE, "len %ld too small to show bar...", len);
    }
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
    char * str = fillBar(NULL, len, pnum);
    printf("%s\n", str);


    free(str);
    return 0;
}
