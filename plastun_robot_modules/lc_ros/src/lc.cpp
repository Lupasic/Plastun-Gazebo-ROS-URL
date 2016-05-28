/*
 * lc.cpp
 *
 *  Created on: 31 июля 2014 г.
 *      Author: viki
 */
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "logiccontrol.h"


int main(int argc, char* argv[])
{
	ros::init(argc, argv, "lc_node");

	ros::NodeHandle priv_nh("~");
	ros::NodeHandle node;
	logic_control l_c(node, priv_nh);

	  // handle callbacks until shut down
	ros::spin();

	return 0;

}
