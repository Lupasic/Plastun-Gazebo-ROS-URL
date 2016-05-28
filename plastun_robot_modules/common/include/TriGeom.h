/*
 * TriGeom.h
 *
 *  Created on: 05.07.2010
 *      Author: andrey
 */

#ifndef TRIGEOM_H_
#define TRIGEOM_H_

#include <math.h>

class TriGeom
{
	int count;
	int center;
	double *SIN;
	double *COS;
	double start;
	double end;
	double resol;
public:
	double getStart() { return start; }
	double getEnd() { return end; }
	double getResolution() { return resol; }
	double getCount() { return count; };
	//get index of zero angle
	int getCenter() { return center; }
	int rad2index(double r)
	{
		return  ((r - start)/ resol );
	}
	double index2rad(int i)
	{
		return start + resol * i;
	}
	double sin(int i) {return SIN[i]; }
	double cos(int i) {return COS[i]; }
	TriGeom(int c, double start = 0, double end = M_PI);
	void init(int c, double start = 0, double end = M_PI);
	TriGeom(){}
	virtual ~TriGeom();
};

#endif /* TRIGEOM_H_ */
