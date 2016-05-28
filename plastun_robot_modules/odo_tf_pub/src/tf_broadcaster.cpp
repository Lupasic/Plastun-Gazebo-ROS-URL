#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include "sensor_msgs/Imu.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/TwistStamped.h"
#include "tf/transform_datatypes.h"
double x = 0.0;
double y = 0.0;
double th = 0.0;
ros::Time last_time;
bool use_imu = false;

tf::Quaternion q;
tf::Vector3 v(0,0,0);
tf::Vector3 w(0,0,0);
tf::Matrix3x3 tf3d;
tf::Quaternion q0;

void odoCallback(const nav_msgs::OdometryPtr & msg)
{
  //ROS_INFO_STREAM("odo");
  //printf("odo %d %d %f\n", msg->header.stamp.sec, msg->header.stamp.nsec, msg->twist.twist.linear.x);
  tf::vector3MsgToTF(msg->twist.twist.linear, v);
  tf::vector3MsgToTF(msg->twist.twist.angular, w);

}


//twist variant for vrep simulator
void twistCallback(const geometry_msgs::TwistStamped & msg)
{
  tf::vector3MsgToTF(msg.twist.linear, v);
  tf::vector3MsgToTF(msg.twist.angular, w);
}


void imuCallback(const sensor_msgs::ImuPtr & msg)
{
//  ROS_INFO_STREAM("imu "<<msg->header.stamp.sec<<" "<<msg->header.stamp.nsec<<" "<<
//      msg->orientation.w<<" "<<msg->orientation.x<<" "<<msg->orientation.y<<" "<<msg->orientation.z <<" "<<
//      msg->linear_acceleration.x<<" "<<msg->linear_acceleration.y<<" "<<msg->linear_acceleration.z<<" "<<
//      msg->angular_velocity.x<<" "<<msg->angular_velocity.y<<" "<<msg->angular_velocity.z);
  q.setValue(
      msg->orientation.x,
      msg->orientation.y,
      0,
      msg->orientation.w);

//  ROS_INFO_STREAM("get q = "<<q.x()<<" "<<q.y()<<" "<<q.z()<<" "<<q.w());
  q = q0*q*q0.inverse();
//  ROS_INFO_STREAM("q0*q*q0inv = "<<q.x()<<" "<<q.y()<<" "<<q.z()<<" "<<q.w());
}

void update_tf(const ros::TimerEvent&)
{
  static tf::TransformBroadcaster odom_broadcaster;
  ros::Time current_time = ros::Time::now();
  float dt = (current_time - last_time).toSec();
  last_time = current_time;

  //first, we'll publish the transform over tf
  static tf::Transform odom_trans;

  if ( !use_imu ) //calculate quaternion from odometry
  {
    static double yaw = 0;
    yaw += w[2]*dt;
    tf3d.setRPY(0, 0, yaw);
  }
  else
  {
    tf3d.setRotation(q);
  }

  odom_trans.setOrigin(odom_trans.getOrigin() + tf3d*v*dt);
  odom_trans.getOrigin().setZ(0);

  odom_trans.setBasis(tf3d);
  ROS_DEBUG_STREAM("publish tf "<<odom_trans.getOrigin().x()<<
                  " "<<odom_trans.getOrigin().y()<<
                  " "<<odom_trans.getOrigin().z());
  tf::StampedTransform sTf(odom_trans, current_time,"odom", "base_link");
  odom_broadcaster.sendTransform(sTf);
}



int main(int argc, char** argv)
{
  try
  {


    ros::init(argc, argv, "odo_tf_broadcaster");
//    tf::Transform tr;
//    tr.setBasis(tf::Matrix3x3(0.0, -1.0, 0.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0));
//    tf::Quaternion qtr = tr.getRotation();
//    ROS_INFO_STREAM(qtr.x()<<" "<<qtr.y()<<" "<<qtr.z()<<" "<<qtr.w());

    q.setRPY(0,0,0);
    ros::NodeHandle node("~");
    node.param("use_imu", use_imu, true);
    if ( use_imu )
    {
      double imu_roll = 0;
      double imu_pitch = 0;
      double imu_yaw = 0;
      node.param("imu_roll", imu_roll, 0.0);
      node.param("imu_pitch", imu_pitch, 0.0);
      node.param("imu_yaw", imu_yaw, 0.0);
      //q0.setRPY(M_PI/2,0,M_PI/2); //values for koblenz
      q0.setRPY(imu_roll*M_PI/180.0, imu_pitch*M_PI/180.0, imu_yaw*M_PI/180.0);
      ROS_INFO_STREAM("q0 = "<<q0.x()<<" "<<q0.y()<<" "<<0<<" "<<q0.w());
    }
    tf3d.setIdentity();

    bool use_twist = false;
    node.param("use_twist", use_twist, false);

    ros::Subscriber odo_sub;
    if (! use_twist )
      odo_sub = node.subscribe("/odo", 100, odoCallback);
    else
      odo_sub = node.subscribe("/twist", 100, twistCallback);

    ros::Subscriber imu_sub;
    if ( use_imu )
      imu_sub = node.subscribe("/imu", 100, imuCallback);
    last_time = ros::Time::now();
    double dt = 0.1;
    node.param("dt", dt, dt);
    ros::Timer timer1 = node.createTimer(ros::Duration(dt), update_tf);

    ros::spin();

  }
  catch (ros::Exception& e)
  {
    ROS_ERROR_STREAM(e.what());
  }
  return 0;
}
;
