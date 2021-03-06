/*
 * COMConnect.cpp
 *
 *  Created on: 04 авг. 2014 г.
 *      Author: viki
 */

#include "COMConnect.h"
#include <stdlib.h>
#include <math.h>
#include <TimeProcs.h>
#include <fcntl.h>

using namespace std;


bool COMConnect::Init(TINIFile & CFG)
{
	srand(time(NULL));
	inifile = CFG.FileName();
	start_time = GetTime();
	timeout = CFG.ReadInt("Logic", "timeout", 100);
	dt = CFG.ReadInt("Logic", "dt", 10);
	twrite = CFG.ReadInt("Logic", "twrite", 0);
	max_status_size = CFG.ReadInt("Logic", "max status size", 100 );
	//robot parameters
	kwh = CFG.ReadFloat("robot", "k_reductor", 17576.0 / 342.0);// = 17576.0 / 342.0;
	//wheel diameter
	dwh = CFG.ReadFloat("robot", "wheel", 0.18);// = 0.18;
	//distance between wheels
	lwh = CFG.ReadFloat("robot", "width", 0.40 );// = 0.400;
	//v [m/sec]of wheel = km * msg->left(right)Motor[rot]
	// 						w		*	r
	km = (2*M_PI / kwh) * (dwh /2);
	x = 0.0;
	y = 0.0;
	th = 0.0;
	vx = 0.0;
	vy = 0.0;
	vth = 0.0;

	current_time = ros::Time::now();
	last_time = ros::Time::now();

	std::string dev = CFG.ReadStr("Logic", "com", "/dev/ttyS0");
	speed_t bspd = B115200;
	switch (CFG.ReadInt("Logic", "bps", 115200)) {
		case 9600:
			bspd = B9600;
			break;
		case 19200:
			bspd = B19200;
			break;
		case 57600:
			bspd = B57600;
			break;
		case 38400:
			bspd = B38400;
			break;
		default:

			break;
	}
	std::cout<<"init COM on "<<dev<<"  speed "<<bspd<<std::endl;
	try
	{
		COM.Open(dev,O_RDWR | O_NOCTTY);

		tcgetattr(COM.Descr(),&oldtio);
		fcntl(COM.Descr(), F_SETFL, 0);
		memcpy(&newtio, &oldtio, sizeof(newtio) );

		// Communication speed (simple version, using the predefined
		// constants)
		//
		cfsetispeed(&newtio, bspd);
		cfsetospeed(&newtio, bspd);

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
		newtio.c_lflag &= ~( ECHO | ICANON | ISIG | XCASE );
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


		tcsetattr(COM.Descr(),TCSANOW,&newtio);
		tcsetattr(COM.Descr(),TCSAFLUSH,&newtio);

	}
	catch (std::exception& )
	{
	        ROS_ERROR_STREAM("failed to init port "<<dev);
		return false;
	}

	  // Set up publishers
          lc_odo_publisher_ = node.advertise<nav_msgs::Odometry>("odo", 10);

	return true;
}

bool COMConnect::readRequest(uint8_t *buf, unsigned int bsize)
{
	controller_msg_t* msg = (controller_msg_t*)buf;
	//read size
	int rsize = 0;

	//read head
	if ( COM.ReadCond(buf + rsize, 1, 0, timeout*1e3) != 1 )
	{
		ROS_WARN_STREAM(" error read header 1 ");
		return false;

	}
//	else std::cout<<"read header 1 = "<<std::hex<<(int)buf[0]<<std::dec<<std::endl;

	if ( COM.ReadCond(buf + rsize + 1, 1, 0, dt*1e3) != 1 )
	{
	        ROS_WARN_STREAM(" error read header 2");
		return false;

	}
//	else std::cout<<"read header 2 = "<<std::hex<<(int)buf[1]<<std::dec<<std::endl;

	if ( msg->start != UARTCMD_MARKER )
	{
	  ROS_WARN_STREAM(" wrong header "<<std::hex<<(int)msg->start<<std::dec);
		return false;
	}

	rsize += sizeof(msg->start);
	//read cmd
	for (unsigned int i = 0; i<sizeof(msg->cmd); i++ )
	{
		if ( COM.ReadCond(buf + rsize + i, 1, 0, dt*1e3) != 1 )
		{
		  ROS_WARN_STREAM(" error read cmd "<<i);
			return false;
		}
	}


	if ( msg->cmd != UARTCMD_REQ_STATE )
	{
	  ROS_WARN_STREAM(" wrong cmd "<<std::hex<<(int)msg->cmd<<std::dec);
		return false;
	}

	rsize +=sizeof(msg->cmd);
	//read size
	for (unsigned int i = 0; i<sizeof(msg->size); i++ )
	{
		if ( COM.ReadCond(buf + rsize + i, 1, 0, dt*1e3) != 1 )
		{
		  ROS_WARN_STREAM(" error read size ");
			return false;
		}
	}

	if ( msg->size != sizeof(controller_msg_t) + MAX_NAVREQDATASIZE -
			sizeof(msg->start) -
				sizeof(msg->cmd) - sizeof(msg->size) )
	{
	  ROS_WARN_STREAM(" wrong size "<<(int)msg->size);
		return false;
	}
	rsize += sizeof(msg->size);

	assert(msg->size  + sizeof(msg->start) +
			sizeof(msg->cmd) + sizeof(msg->size) <= bsize);

	//read all remain
	int res = 1;
	unsigned int i = 0;

	while( res && i < msg->size   )
	{
		res = COM.ReadCond( buf + rsize + i,
				msg->size - i, 0, dt*1e3 );
		if (res) i+=res;
	}

	if ( i != msg->size )
	{
	  ROS_WARN_STREAM(" error read msg ");
		return false;
	}
//	std::cout<<" read message from com success "<<rsize + i<<std::endl;
	//TODO check crc
	return true;
}

void *COMConnect::Main()
{

	while(m_active)
	{

//		tcflush(COM.Descr(), TCIFLUSH);
		static uint8_t buf[ MAX_NAVANSDATASIZE + sizeof(controller_msg_t)];

		//read com port for new message from controller
		if ( !readRequest(buf, sizeof(buf)) )
		{
			int res(0);
			int i(0);
			while( (res = COM.ReadCond(buf, 1, 0, dt*1e3)) )
			{
				;//std::cout<<i++<<" clean read "<<std::hex<<(int)buf[0]<<std::dec<<std::endl;
			}
			continue; //wait next message
		}




		  //analyze input data
		controller_msg_t* cmsg = (controller_msg_t* )buf;

		/*

		//generate DriveMessage
		static DrivesMessage Dmsg;
		static SonarsMessage Smsg;
		clock_gettime( CLOCK_REALTIME, &Dmsg.time );
		Dmsg.moving = cmsg->leftMotor || cmsg->rightMotor;
*/
		float Vl = cmsg->leftMotor * km;
		float Vr = cmsg->rightMotor * km;
		vx = (Vl + Vr) / 2;
		vy = 0.0;
		vth = (Vr - Vl) / 2 / (lwh/2);

		publish_data();

		//process cmd message
//		readCmdMessage(buf + sizeof(controller_msg_t), MAX_NAVANSDATASIZE );

		//set reply
		cmsg->start = UARTCMD_MARKER;
		cmsg->cmd = UARTCMD_ANS_STATE;
		cmsg->size = max_status_size +
				sizeof(controller_msg_t)
				- sizeof(cmsg->cmd) - sizeof(cmsg->start) - sizeof(cmsg->size);


		if ( drvCmd.get() )
		{

                    cmsg->leftMotor = (int)( (drvCmd->linear.x - lwh * drvCmd->angular.z / 2 )/ km );
                    cmsg->rightMotor = (int)( (drvCmd->linear.x + lwh * drvCmd->angular.z / 2 )/ km );
                    if (cmsg->state.flags.automatic) drvCmd.reset();
                    cmsg->state.flags.automatic = 1;
		}
		else
		{
		  cmsg->leftMotor = 0;
                  cmsg->rightMotor = 0;
                  cmsg->state.flags.automatic = 0;
		}


		if ( twrite )
		{
			for(unsigned int i =0; i< sizeof(controller_msg_t) + max_status_size; i++)
			{
				COM.Write( buf+i, 1 );
				usleep(twrite);
			}
		}
		else COM.Write( buf, sizeof(controller_msg_t) + max_status_size );

	}
	return NULL;
}

COMConnect::~COMConnect() {
	// TODO Auto-generated destructor stub
}

void COMConnect::publish_data()
{
    current_time = ros::Time::now();

    //compute odometry in a typical way given the velocities of the robot
    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;

    x += delta_x;
    y += delta_y;
    th += delta_th;

    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //first, we'll publish the transform over tf
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    //odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    odo_data.header.stamp = current_time;
    odo_data.header.frame_id = "odom";

    //set the position
    odo_data.pose.pose.position.x = x;
    odo_data.pose.pose.position.y = y;
    odo_data.pose.pose.position.z = 0.0;
    odo_data.pose.pose.orientation = odom_quat;

    //set the velocity
    odo_data.child_frame_id = "base_link";
    odo_data.twist.twist.linear.x = vx;
    odo_data.twist.twist.linear.y = vy;
    odo_data.twist.twist.angular.z = vth;

    //publish the message
    if (lc_odo_publisher_.getNumSubscribers() > 0)
             lc_odo_publisher_.publish(odo_data);


    last_time = current_time;
}
