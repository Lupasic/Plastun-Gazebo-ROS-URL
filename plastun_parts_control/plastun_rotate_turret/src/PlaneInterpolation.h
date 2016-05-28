/*
 * PlaneInterpolation.h
 *
 *  Created on: 30.10.2013
 *      Author: user
 */

#ifndef PLANEINTERPOLATION_H_
#define PLANEINTERPOLATION_H_

#include <math.h>

class PlaneInterpolation
{
protected:
	//plane factors
	float a, b, c, d;
	float range(float x, float y, float z);
public:
	float getInterpolation(float x, float y);
	//returns true if points are on the line
	bool onLine(float eps = 0.0001)
	{
		return fabs(c) < eps;
	}
	PlaneInterpolation(float x1, float y1, float z1,
			    float x2, float y2, float z2,
			    float x3, float y3, float z3);
};

#endif /* PLANEINTERPOLATION_H_ */
