/*
 * PlaneInterpolation.cpp
 *
 *  Created on: 30.10.2013
 *      Author: user
 */

#include "PlaneInterpolation.h"
#include <iostream>
float PlaneInterpolation::getInterpolation(float x, float y)
{
	return (-a*x  -b*y - d )/c;
}

float PlaneInterpolation::range(float x, float y, float z)
{
	return a*x +b*y + c*z +d;
}

PlaneInterpolation::PlaneInterpolation(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3):a(0),b(0),c(0),d(0)
{
	//q 1  p 2  r 3
	a = (y1-y2)*(z3-z2) - (y3-y2)*(z1-z2);
	b = (z1-z2)*(x3-x2) - (x1-x2)*(z3-z2);
	c = -(x3-x2)*(y1-y2) + (x1-x2)*(y3-y2);

//	std::cout<<" a = "<<a<<" b = "<<b<<" c = "<<c<<std::endl;
	//3d coefficient in plane ax+by+cz+D=0
//	d = -x2*(y1-y2)*(z3-z2) -
//					y2*(x3-x2)*(z1-z2) -
//						z2*(x1-x2)*(y3-y2)
//			+ z2*(y1-y2)*(x3-x2) +
//				y2*(x1-x2)*(z3-z2) +
//					x2*(z1-z2)*(y3-y2);
	d = -x1*a -y1*b - z1*c;
//	std::cout<<" d = "<<d<<std::endl;
//	std::cout<<" test "<<range(x1,y1, z1)<<std::endl;
//	std::cout<<" test "<<range(x2,y2, z2)<<std::endl;
//	std::cout<<" test "<<range(x3,y3, z3)<<std::endl;
}
