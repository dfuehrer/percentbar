#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <time.h>
#include "percentlib.h"

typedef struct cputimes_s{
    unsigned long active;
    unsigned long total;
} cputimes_t;

int main(const int argc, const char * const argv[]){

    // get the number of cores from the /dev/cpu dir
    unsigned int numCores = 0;
    //DIR * cpudir = opendir("/dev/cpu");
    //for(struct dirent * de; (de = readdir(cpudir)) != NULL; ){
    //    int thisNum = atoi(de->d_name);
    //    if(thisNum > numCores){
    //        numCores = thisNum;
    //    }
    //}
    //closedir(cpudir);
    //++numCores;
    FILE * cpuinfof = fopen("/proc/cpuinfo", "r");
    int numRead = 0;
    size_t n = 0;
    char * cpuinfoLine = NULL;
    for(ssize_t len = getline(&cpuinfoLine, &n, cpuinfof); (numRead = sscanf(cpuinfoLine, "siblings\t: %u", &numCores)) == 0; len = getline(&cpuinfoLine, &n, cpuinfof));
    free(cpuinfoLine);
    fclose(cpuinfof);
    int coresChars;
    if(numCores < 10){
        coresChars = 1;
    }else if(numCores < 100){
        coresChars = 2;
    }else if(numCores < 1000){
        coresChars = 3;
    }else{
        unsigned int comp = 10000;
        for(coresChars = 4; numCores >= comp && comp < 1000000000; comp *= 10, ++coresChars);
    }
    // header is cpu + number + space
    int headerSize = 3 + 1 + coresChars;

    // get the amount of time to sleep if not 1s
    double sleeptimed = 1;
    if(argc > 1){
        errno = 0;
        char * tmpPtr = NULL;
        sleeptimed = strtod(argv[1], &tmpPtr);
        if(tmpPtr == argv[1]){
            error(EXIT_FAILURE, errno, "could not interpret '%s' as a double for time to sleep", argv[1]);
        // if the time is < .01 then thats too little time for /proc/stat to update (100Hz)
        }else if(errno != 0 || sleeptimed < 0.02){
            error(EXIT_FAILURE, errno, "'%s' is not a valid sleep time", argv[1]);
        }
        printf("%g\n", sleeptimed);
    }
    struct timespec sleeptime;
    sleeptime.tv_sec = (long)sleeptimed;
    sleeptime.tv_nsec = (sleeptimed - sleeptime.tv_sec) * 1000000000;

    // get the window size to use for the bars
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    int len = w.ws_col;

    cputimes_t * firstTimes = (cputimes_t *) calloc(numCores + 1, sizeof (cputimes_t));
    cputimes_t firstTime;

    // read the cpu times data in from /proc/stat and save it
    FILE * statf = fopen("/proc/stat", "r");
    unsigned int cpu;
    unsigned long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    fscanf(statf, "cpu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
    firstTime.active = user + system + nice + softirq + steal;
    firstTime.total  = firstTime.active + idle + iowait;
    for(unsigned int i = 0; i < numCores; ++i){
        fscanf(statf, "cpu%d %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n", &cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
        if(i != cpu){
            fprintf(stderr, "on cpu #%d but got cpu%u\n", i, cpu);
        }
        firstTimes[i].active = user + system + nice + softirq + steal;
        firstTimes[i].total  = firstTimes[i].active + idle + iowait;
    }
    fclose(statf);

    // sleep to get the time diff needed to get a percent
    nanosleep(&sleeptime, &sleeptime);

    statf = fopen("/proc/stat", "r");
    unsigned long active, total;
    fscanf(statf, "cpu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
    active = user + system + nice + softirq + steal;
    total  = active + idle + iowait;
    double percent = (double)(active - firstTime.active) * 100 / (double)(total - firstTime.total);
    if(percent > 100){
        percent = 100;
    }
    char * pbar = fillBar(NULL, len-6, percent);
    printf("cpu   %s\n", pbar);
    for(int i = 0; i < numCores; ++i){
        fscanf(statf, "cpu%d %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n", &cpu, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
        if(i != cpu){
            fprintf(stderr, "on cpu #%d but got cpu%u\n", i, cpu);
        }
        active = user + system + nice + softirq + steal;
        total  = active + idle + iowait;
        percent = (double)(active - firstTimes[i].active) * 100 / (double)(total - firstTimes[i].total);
        if(percent > 100){
            percent = 100;
        }
        fillBar(pbar, len-headerSize, percent);
        printf("cpu%*d %s\n", coresChars, i, pbar); // yes this assumes i have less than 100 threads
    }
    fclose(statf);
    free(pbar);

    return 0;
}
