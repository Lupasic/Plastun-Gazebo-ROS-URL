/*
 * matrixHelper.h
 *
 *  Created on: 06.07.2010
 *      Author: andrey
 */

#ifndef MATRIXHELPER_H_
#define MATRIXHELPER_H_

template <int size>
union TMatrix
	{
		double matrix[size][size];
		double array[size*size];
	};
//make matrix from angles in radians
void euler2matrix(float pitch, float roll, float yaw, TMatrix<3>& M );

#endif /* MATRIXHELPER_H_ */
