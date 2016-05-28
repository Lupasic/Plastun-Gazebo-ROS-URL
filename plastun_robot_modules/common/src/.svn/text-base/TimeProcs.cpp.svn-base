#include "TimeProcs.h"
#include <time.h>

double GetTime()
{
	timespec t;
	clock_gettime( CLOCK_REALTIME, &t );

	return ( double )t.tv_sec + ( double )( t.tv_nsec / 1000 ) / 1e6;
}

//delta in seconds
double deltaS(struct timespec &t0, struct timespec &t1)
{
	return (double)( t1.tv_sec -t0.tv_sec ) + (double)( (t1.tv_nsec -t0.tv_nsec) / 1000 )/ 1e6 ;
}

//delta in ms
double deltaMs(struct timespec &t0, struct timespec &t1)
{
	return (double)( t1.tv_sec -t0.tv_sec )*1000 + (double)( (t1.tv_nsec -t0.tv_nsec)  )/ 1e6 ;
}

//in usec
long int delta(struct timespec &t0, struct timespec &t1)
{
	return ( t1.tv_sec -t0.tv_sec ) * 1000000000L + (t1.tv_nsec -t0.tv_nsec)  ;
}

//delta in seconds
double deltaS(struct timespec &t0)
{
	timespec t1;
	clock_gettime( CLOCK_REALTIME, &t1 );
	return deltaS(t0, t1 );
}

//delta in ms
double deltaMs(struct timespec &t0)
{
	timespec t1;
	clock_gettime( CLOCK_REALTIME, &t1 );
	return deltaMs(t0, t1 );
}

//in usec
long int delta(struct timespec &t0)
{
	timespec t1;
	clock_gettime( CLOCK_REALTIME, &t1 );
	return delta(t0, t1 );
}
