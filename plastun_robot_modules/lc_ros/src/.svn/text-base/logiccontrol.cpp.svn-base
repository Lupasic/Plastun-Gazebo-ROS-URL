/*
 * logiccontrol.cpp
 *
 *  Created on: 05 авг. 2014 г.
 *      Author: viki
 */

#include "logiccontrol.h"

logic_control::logic_control(ros::NodeHandle node, ros::NodeHandle priv_nh)
{


	  // get parameters using private node handle
//	  priv_nh.param("cell_size", m_per_cell_, 0.2);
//	//  priv_nh.param("full_clouds", full_clouds_, false);
//	  priv_nh.param("map_size", map_sz, 40.0);
//	  grid_dim_ = (int)(map_sz/m_per_cell_);
//	//  priv_nh.param("grid_dimensions", grid_dim_, 320);
//	  priv_nh.param("height_threshold", height_diff_threshold_, 0.5);
//
//	  ROS_INFO_STREAM("height map parameters: "
//	                  << grid_dim_ << "x" << grid_dim_ << ", "
//	                  << m_per_cell_ << "m cells, "
//	                  << height_diff_threshold_ << "m threshold ");

	  std::string cfg;
	  std::string fullName;
	  fullName="cfgfile";
	  ROS_INFO_STREAM("fullName="<<fullName);
	  if ( !node.getParam(fullName, cfg))
	  {
	    cfg = "mobile.cfg";
	  }
	  ROS_INFO_STREAM("log name is: " <<cfg);

	  TINIFile CFG(cfg);
	   try
	   {
		  CFG.Load();
	   }
	   catch (std::logic_error& E)
	   {
		  ROS_ERROR_STREAM(" error load config"<<E.what());
		  exit(0);
	   }
	   COM_cn.node = node;
	   COM_cn.Init(CFG);
	   COM_cn.Start();

	  // subscribe to twist data
	  twist_dater_ = node.subscribe("/cmd_vel",100, &logic_control::processData2,this,
	                                  ros::TransportHints().tcpNoDelay(true));

}


/** twist data input callback */
void logic_control::processData(const geometry_msgs::Twist& vel_cmd)
{
//  if (lc_odo_publisher_.getNumSubscribers() == 0)
//     return;
//
////  odo_data_.header.seq = scan->header.seq;
////  odo_data_.header.stamp.sec = floor(scan->header.stamp / 1000.0);
////  odo_data_.header.stamp.nsec = (scan->header.stamp - obs_map_.header.stamp.sec)* 1000;
////  odo_data_.header.frame_id = scan->header.frame_id;
//
//
//
//  if (lc_odo_publisher_.getNumSubscribers() > 0)
//	  lc_odo_publisher_.publish(odo_data);

}

/** twist data input callback */
void logic_control::processData2(const geometry_msgs::TwistPtr& vel_cmd_ptr)
{
  ROS_INFO_STREAM("cmd = "<<vel_cmd_ptr->linear.x<<" "<<vel_cmd_ptr->angular.z);
    COM_cn.setCmd(vel_cmd_ptr);
}


logic_control::~logic_control()
{
	// TODO Auto-generated destructor stub
}

