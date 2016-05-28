/*
 * ahrs.h
 *
 *  Created on: 13.10.2010
 *      Author: andrey
 */

#ifndef AHRS_H_
#define AHRS_H_

//commands for ahrs

enum msgID
{
	DATA=112,
	ADD=114
};

#pragma pack(1)

struct BINS_NavData
{
	union
	{
		uint32_t data;
		struct
		{
			bool ins_err:1;
			bool sns_err:1;
			bool magnit_err:1;
			bool baro_err:1;
			bool sns_correct:1;
			bool dvs_correct:1;
			bool motionless:1;
			bool correction_inprocess:1;
			bool correction_by_sns:1;
			bool lattitude_set:1;
			bool angle_latitude_correction:1;
			bool zero_vel_correction:1;
			bool jerck:1;
			bool double_hyrocompass:1;
			bool double_hyrocompass2:1;
			bool ready:1;
			bool autonomous:1;
			bool acc_correction:1;
			bool odo_correction1:1;
			bool odo_correction2:1;
			bool res1:1;
			bool res2:1;
			bool res3:1;
			bool res4:1;
			bool res5:1;
			bool res6:1;
			bool res7:1;
			bool res8:1;
			bool res9:1;
			bool res10:1;
			bool res11:1;
			bool res12:1;
		}bits;
	}state;
	float accx;
	float accy;
	float accz;
	float wx;
	float wy;
	float wz;
	float roll;
	float yaw;
	float pitch;
	int32_t latitude;
	int32_t longitude;
	float height;
};

struct BINS_AddData
{
	float yaw;
	float roll;
	float pitch;
	float Ve;
	float Vn;
	float Vh;
	float xSK42;
	float ySK42;
	float zSK42;
	float headingSK42;
	float accRoll;
	float accPitch;
};



#pragma pack()

#endif /* AHRS_H_ */
