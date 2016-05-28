/*
 * matrixHelper.cpp
 *
 *  Created on: 06.07.2010
 *      Author: andrey
 */

#include <matrixHelper.h>
#include <math.h>

void euler2matrix(float pitch, float roll, float yaw, TMatrix<3>& R )
{
	//calculate Orientation matrix

	double ca = cos( yaw ); double sa = sin( yaw );
	double cb = cos( pitch ); double sb = sin( pitch );
	double cy = cos( roll) ; double sy = sin( roll );
	//rotation matrix from Lavalle
	R.matrix[0][0] =  ca*cb; 	 R.matrix[0][1] =  ca*sb*sy - sa*cy; R.matrix[0][2] =  ca*sb*cy + sa*sy;
	R.matrix[1][0] = sa*cb;     R.matrix[1][1] = sa*sb*sy + ca*cy;  R.matrix[1][2] = sa*sb*cy - ca*sy;
	R.matrix[2][0] = -sb ;      R.matrix[2][1] = cb*sy ;              R.matrix[2][2] = cb*cy ;
//	std::cout<<"robot orientation "<<std::endl
//        << R.matrix[0][0] <<"  "<< R.matrix[1][0]<<"  "<< R.matrix[2][0] <<std::endl
//        << R.matrix[0][1] <<"  "<< R.matrix[1][1]<<"  "<< R.matrix[2][1] <<std::endl
//        << R.matrix[0][2] <<"  "<< R.matrix[1][2]<<"  "<< R.matrix[2][2] <<std::endl;
}
