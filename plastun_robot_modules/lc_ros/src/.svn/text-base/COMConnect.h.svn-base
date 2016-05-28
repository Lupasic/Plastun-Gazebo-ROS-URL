/*
 * COMConnect.h
 *
 *  Created on: 04 авг. 2014 г.
 *      Author: viki
 */

#ifndef COMCONNECT_H_
#define COMCONNECT_H_

#include <File.h>
#include <inifile.h>
#include <TThread.h>
#include <crc.h>
#include <termios.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "COMcommands.h"
#include <iostream>
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>

class COMConnect: public TThread
{

protected:
	//robot params
	//reductor
	float kwh;// = 17576.0 / 342.0;
	//wheel diameter
	float dwh;// = 0.18;
	//distance between wheels
	float lwh;// = 0.400;
	//v [m/sec]of wheel = km * msg->left(right)Motor[rot]
	// 						w		*	r
	float km;// = (2*M_PI / kwh) * (dwh /2);
	struct termios oldtio, newtio;
	volatile bool m_active;
	TFile COM;
	TCRC16_LHA CRC;
	int timeout;
	float start_time;
	//inter words timeout, ms
	int dt;
	//dt for write a byte
	int  twrite;
	//for settings
	std::string inifile;
	//maximum size of status message
	unsigned int max_status_size;

	double x;
	double y;
	double th;
	double vx;
	double vy;
	double vth;

	ros::Time current_time;
	ros::Time last_time;
	geometry_msgs::TransformStamped odom_trans;
        geometry_msgs::TwistPtr drvCmd;
	nav_msgs::Odometry odo_data;
	bool readRequest(uint8_t *buf, unsigned int size);
	bool isActive() { return m_active; }
	void setActive(bool act = true) { m_active = act; }
	virtual void* Main() ;
	void publish_data();
public:
	void setCmd(geometry_msgs::TwistPtr ptr)
	{
	  drvCmd = ptr;
	}
	void Start()
	{
		if (m_active) return ;
		m_active = true;
		Run();
	}
	void Stop()
	{
		m_active = false;
	}
	ros::Publisher lc_odo_publisher_;
	tf::TransformBroadcaster odom_broadcaster;
	ros::NodeHandle node;
	bool Init( TINIFile & CFG);
	COMConnect():m_active(false){};
	virtual ~COMConnect();
};

#endif /* COMCONNECT_H_ */
