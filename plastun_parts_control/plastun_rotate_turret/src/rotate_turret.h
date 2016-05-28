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
    actionlib::SimpleActionServer<plastun_rotate_turret::angleAction> *angl;
    std::string action_name;
    plastun_rotate_turret::angleFeedback feedback;
    plastun_rotate_turret::angleResult result;
    plastun_rotate_turret::angleGoal::ConstPtr goal;
    ros::Subscriber turret_x_cur_pos, turret_y_cur_pos;
    ros::Publisher turret_x_command, turret_y_command;
    double eps;
    std_msgs::Float64 check;
    bool fl_x, fl_y;
    int kostil;
    ros::Timer timer;

public:
    Rotate_turret(std::string name);
    ~Rotate_turret() {}
    void goal_R();
    void preempt_R();
    void turret_x_pos(const control_msgs::JointControllerState &msg);
    void turret_y_pos(const control_msgs::JointControllerState &msg);
    void timer_callback(const ros::TimerEvent& event);

};
