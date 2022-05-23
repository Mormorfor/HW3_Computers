// implementation of clock.h functions in Windows

#ifdef _WIN32
#include <profileapi.h>
#include "clock.h"
static LARGE_INTEGER start_ticks;
void start_counter(){
    QueryPerformanceCounter(&start_ticks);
}

double get_counter(){
    // return the time in  micro seconds
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    return (ticks.QuadPart - start_ticks.QuadPart)/10.0; // HACK:assuming here that QPC FREQ = 10MHz
}

void start_comp_counter(){
    start_counter();
}

double get_comp_counter(){
    return get_counter();
}

#endif
