#include <ros/ros.h>
#include "geometry_msgs/Pose.h"
#include "control_msgs/JointControllerState.h"
#include "std_msgs/Float64.h"
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include <tf2_msgs/TFMessage.h>
#include <actionlib/server/simple_action_server.h>
//#include "../../../../devel/include/plastun_rotate_turret/angleAction.h"
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_rotate_turret/angleAction.h"
#include <cmath>

class Rotate_turret
{
protected:
    ros::NodeHandle nh;
    actionlib::SimpleActionServer<plastun_rotate_turret::angleAction> *rt_server;
    std::string action_name;
    plastun_rotate_turret::angleGoal::ConstPtr goal;
    plastun_rotate_turret::angleResult result;
    plastun_rotate_turret::angleFeedback feedback;
    ros::Subscriber turret_yaw_cur_pos, turret_pitch_cur_pos;
    ros::Publisher turret_yaw_command, turret_pitch_command;
    double eps;
    std_msgs::Float64 check;
    bool fl_yaw, fl_pitch;
    ros::Timer timer;
    float analysis_yaw, analysis_pitch;

public:
    Rotate_turret(std::string name);
    ~Rotate_turret() {}
    void goal_R();
    void preempt_R();
    void turret_yaw_pos(const control_msgs::JointControllerState &msg);
    void turret_pitch_pos(const control_msgs::JointControllerState &msg);
    void timer_callback(const ros::TimerEvent& event);

};
