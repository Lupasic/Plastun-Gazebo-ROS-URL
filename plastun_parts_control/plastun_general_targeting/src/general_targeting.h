#include <ros/ros.h>
#include "geometry_msgs/Pose.h"
#include "control_msgs/JointControllerState.h"
#include "std_msgs/Float64.h"
#include <tf/transform_listener.h>
#include <tf/tf.h>
#include <tf2_msgs/TFMessage.h>
#include <actionlib/server/simple_action_server.h>
//#include "../../../../devel/include/plastun_rotate_turret/angleAction.h"
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_general_targeting/access_targetingAction.h"
#include <cmath>

class General_targeting
{
protected:
    ros::NodeHandle nh;
    tf::TransformListener *listener;
    actionlib::SimpleActionServer<plastun_general_targeting::access_targetingAction> *gt_server;
    std::string action_name;
    plastun_general_targeting::access_targetingGoal::ConstPtr goal;
    plastun_general_targeting::access_targetingResult result;
    float lengh_to_cam_xy, lengh_to_cam_yz;
    float lengh_to_target;
    float cur_pos_x, cur_pos_y;

public:
    General_targeting(std::string name);
    ~General_targeting() {}
    void goal_R();
    void preempt_R();
    void main_calculation();
};
