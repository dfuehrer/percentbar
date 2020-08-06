#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int num = pnum * len / 100;
    char * str = (char *) malloc(sizeof(char) * (len+7));
    str[0] = '|';
    str[len+1] = '|';
    memset(str+1, (int) '=', num);
    memset(str+1+num, (int) '-', len-num);
    sprintf(str+len+2, "%3d%%\0", pnum);
    puts(str);


    return 0;
}
