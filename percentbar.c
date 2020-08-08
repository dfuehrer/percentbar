#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* #include <unistd.h> */


int fillBar(char * str, int len, int pnum);

//int main(char ** argv, int argc){
int main(int argc, char ** argv){
    // TODO finish my parser so i can use it here
    int len = 15;
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
    return pnum > 100;
}
