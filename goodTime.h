
//This was made to get elapsed time rather than clock() time which always worked on windows but apparently dosn't work on Ubuntu because specifications yadayadayada

#include<stdlib.h>
#include<sys/time.h>

struct timeval goodTimeStart;

void beginTimer(){
    gettimeofday(&goodTimeStart, NULL);
}

double readTimer(){
    struct timeval goodTimeEnd;
    gettimeofday(&goodTimeEnd, NULL);
    double rv = (goodTimeEnd.tv_sec - goodTimeStart.tv_sec) * 1000.0;   //sec to ms
    rv += (goodTimeEnd.tv_usec - goodTimeStart.tv_usec) / 1000.0;       //us to ms
    return rv;
}