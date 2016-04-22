#include <ros/ros.h>
#include "geometry_msgs/Pose.h"
#include "std_msgs/Float64.h"
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include <tf2_msgs/TFMessage.h>

class Rotate_turret
{
protected:
    ros::NodeHandle nh;
    ros::Subscriber target_pose_im, current_pose;
    ros::Publisher turret_camera_x, turret_camera_y;
    geometry_msgs::Pose target, im_target, cur_pose;
    std_msgs::Float64 angle_x,angle_y;
    double Leng, a_x;


public:
    Rotate_turret(double x, double y);
    void rotate();
    ~Rotate_turret();
    void update();

};
