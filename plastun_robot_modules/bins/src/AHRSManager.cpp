/*
 * AHRSManager.cpp
 *
 *  Created on: 11.10.2010
 *      Author: andrey
 */

#include "AHRSManager.h"
#include <sys/uio.h>
#include <TimeProcs.h>
#include <sensor_msgs/Imu.h>
#include <iostream>
#include <tf/transform_datatypes.h>

#define COUT(x) ROS_INFO_STREAM("AHRS: "<<x)

bool AHRSManager::checkSum(uint16_t* buf, unsigned int size, uint16_t chk)
{

	return true;
}

uint16_t AHRSManager::sum(uint16_t* buf, unsigned int size)
{
	uint16_t sum (0);


	return sum;
}

void AHRSManager::flush()
{
	uint8_t buf[256];
	int res = 0;
	while(1)
	{
		res = ahrs.ReadCond(buf, 256,  0, tpar );
		COUT("flush "<<res);
		if ( res != 256 )
		{
			return ;
		}

	}
}


bool AHRSManager::init(TINIFile& CFG)
{
	//timeout for readings
	tpar = CFG.ReadInt("BINS", "tpar",2000000 );
	tdat = CFG.ReadInt("BINS", "tdat", 10000 );
	dev_roll = CFG.ReadFloat("BINS", "dev roll", 0);
	dev_pitch = CFG.ReadFloat("BINS", "dev pitch", 0);
	std::string dev = CFG.ReadStr("BINS", "dev", "/dev/ttyS0");
	speed_t bspd = B230400;
	std::cout<<"init BINS on "<<dev<<std::endl;
	try
	{
		ahrs.Open(dev,O_RDWR | O_NOCTTY);

		tcgetattr(ahrs.Descr(),&oldtio);

//		std::cout<<" old = "<<std::hex<<" c_flag "<<oldtio.c_cflag<<
//				" iflag "<<oldtio.c_iflag<<"  oflag ="<<oldtio.c_oflag<<
//				"  lflag "<<oldtio.c_lflag<<std::dec<<std::endl;
//		std::cout<<" Vmin "<<(int)oldtio.c_cc[VMIN] <<"  Vmax "<<(int)oldtio.c_cc[VTIME]<<std::endl;

		fcntl(ahrs.Descr(), F_SETFL, 0);

		bzero(&newtio, sizeof(newtio));

		// Communication speed (simple version, using the predefined
		// constants)
		//
		if  ( cfsetispeed(&newtio, bspd) != 0)
		{
			COUT ( " can not set baud "<< bspd);
			return false;
		}
		if ( cfsetospeed(&newtio, bspd) != 0 )
		{
			COUT (  " can not set baud "<< bspd);
			return false;
		}

		// Input flags - Turn off input processing
		// convert break to null byte, no CR to NL translation,
		// no NL to CR translation, don't mark parity errors or breaks
		// no input parity check, don't strip high bit off,
		// no XON/XOFF software flow control
		//
//		newtio.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
//		                    INLCR | PARMRK | INPCK | ISTRIP | IXON);
		newtio.c_iflag &=  ~(IGNBRK | BRKINT | IGNPAR | PARMRK | INPCK | ISTRIP
                | INLCR | IGNCR | ICRNL | IXON | IXOFF | IUCLC | IXANY | IMAXBEL);

		//
		// Output flags - Turn off output processing
		// no CR to NL translation, no NL to CR-NL translation,
		// no NL to CR translation, no column 0 CR suppression,
		// no Ctrl-D suppression, no fill characters, no case mapping,
		// no local output processing
		//
		// newtio.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
		//                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
		newtio.c_oflag &= ~OPOST;
		//
		// No line processing:
		// echo off, echo newline off, canonical mode off,
		// extended input processing off, signal chars off
		//
		//newtio.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
		newtio.c_lflag &= ~(ECHO | ICANON | ISIG | XCASE );
		//
		// Turn off character processing
		// clear current char size mask, no parity checking,
		// no output processing, force 8 bit input
		//
		newtio.c_cflag |= (CLOCAL | CREAD);

		newtio.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
		newtio.c_cflag |= CS8;
		//
		// One input byte is enough to return from read()
		// Inter-character timer off
		//
		newtio.c_cc[VMIN]  = 1;
		newtio.c_cc[VTIME] = 0;


		tcsetattr(ahrs.Descr(),TCSANOW,&newtio);
		tcsetattr(ahrs.Descr(),TCSAFLUSH,&newtio);

	}
	catch (std::exception& )
	{
		COUT("failed to init port "<<dev);
		return false;
	}


	return true;
}

int AHRSManager::receiveMessage(void* buf, unsigned int size)
{
	uint8_t* curBuf = (uint8_t*)buf;
	uint16_t* curBufWord = (uint16_t*) buf;
	bool sync = false;
	//wait synch bytes
	while ( m_active )
	{
		//wait 2 sync bytes

		int rbytes = ahrs.ReadCond(curBuf, 1,  0, tpar );
		if (!rbytes )
		{
			COUT(" wait sync1 no data");
			continue;
		}
		if (curBuf[0] != 0xAA )
		{
			COUT("wait sync1 "<<(int)curBuf[0]);
			continue;
		}
		rbytes = ahrs.ReadCond(curBuf, 1,  0, tpar );
		if (!rbytes )
		{
			COUT(" wait sync2 no data");
			continue;
		}
		if (curBuf[0] != 0xAA )
		{
			COUT("wait sync2 "<<(int)curBuf[0]);
			continue;
		}

		sync = true;
		break;
	}
	if ( !sync )
		return -1;
	int rbytes = ahrs.ReadCond(curBuf, 1,  0, tpar );
	if ( curBuf[0] > size )
	{
		COUT("wrong size "<<(int)curBuf[0]);
		return -1;
	}
	//else
	int msg_size = (int)curBuf[0];
	//COUT("message size = "<< msg_size);
	rbytes = 0;
	while (rbytes < msg_size)
	{
		int res =  ahrs.ReadCond(curBuf + rbytes, msg_size - rbytes,  0, tpar );
		if ( res == 0 )
		{
			COUT("no data");
			return -1;
		}
		rbytes += res;
		//COUT("get "<<res);
	}
	if ( !checkSum(curBufWord, msg_size/2 - 1, *(curBufWord + msg_size - 1)) )
	{
		COUT("wrong check sum");
		return -1;
	}
	uint8_t id = (int)curBuf[0];
	//COUT( "msg id = "<< (int)curBuf[0]);
	return id;
}

bool AHRSManager::sendMessage(void* buf, unsigned int size)
{

	return true;
}




void* AHRSManager::Main()
{

	flush();
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);

	while( m_active )
	{
		uint8_t msg[256];

		int id = receiveMessage(msg, 256);
		if ( id == DATA )
		{
			//COUT("size of DATA "<<sizeof(BINS_NavData));
			timespec tc;
			clock_gettime(CLOCK_REALTIME, &tc);

			BINS_NavData* data = (BINS_NavData*)(msg+1);
			if (verbose)
			{
				COUT("dt = "<<deltaMs(tc, t));
				t = tc;
				COUT(" ready = "<<data->state.bits.ready);
				COUT(" acc \t"<<data->accx<<"\t"<<data->accy<<"\t"<<data->accz);
				COUT(" w   \t"<<data->wx<<"\t"<<data->wy<<"\t"<<data->wz);
				COUT("rpy  \t"<<data->roll<<"\t"<<data->pitch<<"\t"<<data->yaw);
			}
			sensor_msgs::Imu imu;
			static int counter = 0;
			counter++;
                        imu.header.seq = counter;
                        imu.header.stamp = ros::Time::now();//floor(timestamp / 1000.0) ;
                        imu.header.frame_id = "imu";

			float yaw = data->yaw*M_PI/180;

			float pitch = M_PI - data->roll*M_PI/180;
			if ( pitch > M_PI )
				pitch -= 2*M_PI;
			else if ( pitch < -M_PI )
				pitch += 2*M_PI;
			float roll = - data->pitch*M_PI/180;

			pitch -= dev_pitch;
			roll  -= dev_roll;
			tf::Quaternion q;
                        q.setRPY(roll, pitch, yaw);

                        tf::quaternionTFToMsg(q, imu.orientation);

                        imu.linear_acceleration.x = data->accz*9.8;
                        imu.linear_acceleration.y = -data->accx*9.8;
                        imu.linear_acceleration.z = data->accy*9.8;

			imu.angular_velocity.x = data->wz*M_PI/180.0;
			imu.angular_velocity.y = -data->wx*M_PI/180.0;
			imu.angular_velocity.z = -data->wy*M_PI/180.0;


			if ( verbose )
			{
				COUT(" msg rpy \t "<<roll<<"\t"<<pitch<<"\t"<<yaw);
				COUT("msg acc \t"<<imu.linear_acceleration);
				COUT("msg w   \t"<<imu.angular_velocity);
			}

			pub.publish(imu);
		}
		else
		{
			;//COUT("id = "<<id);
		}
	}
	COUT("final reset");

	return NULL;
}

AHRSManager::~AHRSManager()
{

	stopManager();
	waitfor();

	if (ahrs.Descr() != -1)
	{
		//tcsetattr(ahrs.Descr(),TCSANOW,&oldtio);
		//COUT("return settings");
	}

}
