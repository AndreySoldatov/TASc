#ifndef TAS_TIME
#define TAS_TIME

#include <stdlib.h>
#include <time.h>

typedef struct Clock {
    long long startNanos;
} Clock;

long long timeNanosFromBoot() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

Clock clockNew() {
    Clock cl = {timeNanosFromBoot()};
    return cl;
}

double clockGetElapsedTime(Clock cl) {
    return (double)(timeNanosFromBoot() - cl.startNanos) / 1000000000.;
}

double clockRestart(Clock * cl) {
    double time = (double)(timeNanosFromBoot() - cl->startNanos) / 1000000000.;
    cl->startNanos = timeNanosFromBoot();
    return time;
}

#endif