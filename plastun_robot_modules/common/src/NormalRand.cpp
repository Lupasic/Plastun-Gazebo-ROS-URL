#include "NormalRand.h"
#include <math.h>
#include <stdlib.h>

double Rand01()
{
	return ( double )rand() / ( RAND_MAX - 1 );
}

double NormalRand( double median, double sigma )
{
    double u;
    double v;
    double sum;

    while( 1 )
    {
        u = Rand01() * 2.0 - 1.0;
        v = Rand01() * 2.0 - 1.0;
        
        sum = u * u + v * v;
        if( sum < 1 && sum > 0 )
        {
            sum = sqrt( -2 * log( sum ) / sum );
			return u * sum * sigma + median;
        }
    }
    
    return 0;
}


