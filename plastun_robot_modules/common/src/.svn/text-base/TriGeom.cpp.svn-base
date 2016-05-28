/*
 * TriGeom.cpp
 *
 *  Created on: 05.07.2010
 *      Author: andrey
 */

#include "TriGeom.h"

void TriGeom::init(int c, double start , double end )
{
	count = c;
	this->start =start;
	this->end = end;
	resol = ( (end - start)/ (c - 1) );

	SIN = new double [c];
	COS = new double [c];

	center = (int)floor(-start / resol + 0.5);
	double a = start;
	double da = resol;

	for (int i = 0; i < c; i++, a+=da)
	{
		SIN[i] = ::sin(a); COS[i] = ::cos(a);
	}
}

TriGeom::TriGeom(int c, double _start , double _end )
{
	init(c,_start, _end);
}

TriGeom::~TriGeom()
{
	delete SIN;
	delete COS;
}
