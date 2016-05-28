#ifndef TIMEPROCS_H_
#define TIMEPROCS_H_

#include <time.h>

// Функция возвращает время в секундах
double GetTime();

//delta in seconds
double deltaS(struct timespec &t0, struct timespec &t1);

//delta in ms
double deltaMs(struct timespec &t0, struct timespec &t1);

//in usec
long int delta(struct timespec &t0, struct timespec &t1);


//deltas with current sys time
//delta in seconds
double deltaS(struct timespec &t);

//delta in ms
double deltaMs(struct timespec &t);

//in usec
long int delta(struct timespec &t);

#endif /*TIMEPROCS_H_*/
