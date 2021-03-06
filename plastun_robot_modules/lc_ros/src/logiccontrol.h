/*
 * logiccontrol.h
 *
 *  Created on: 05 авг. 2014 г.
 *      Author: viki
 */

#ifndef LOGICCONTROL_H_
#define LOGICCONTROL_H_

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <File.h>
#include <inifile.h>
#include "COMConnect.h"

class logic_control
{
public:
	void processData(const geometry_msgs::Twist& vel_cmd);
	void processData2(const geometry_msgs::TwistPtr& vel_cmd_ptr);
	logic_control(ros::NodeHandle node, ros::NodeHandle private_nh);
	COMConnect COM_cn;
	virtual ~logic_control();
	  // ROS topics
	ros::Subscriber twist_dater_;

};

#endif /* LOGICCONTROL_H_ */
