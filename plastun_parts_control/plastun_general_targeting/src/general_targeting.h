#include <ros/ros.h>
#include "geometry_msgs/Pose.h"
#include "control_msgs/JointControllerState.h"
#include "std_msgs/Float64.h"
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include <tf2_msgs/TFMessage.h>
#include <actionlib/server/simple_action_server.h>
//#include "../../../../devel/include/plastun_rotate_turret/angleAction.h"
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_general_targeting/access_targetingAction.h"

class General_targeting
{
protected:
    ros::NodeHandle nh;
    actionlib::SimpleActionServer<plastun_general_targeting::access_targetingAction> *target;
    std::string action_name;
    plastun_general_targeting::access_targetingResult result;
    //plastun_general_targeting::access_targetingGoal::ConstPtr goal;
    const static double eps = 0.02;

public:
    General_targeting(std::string name);
    ~General_targeting() {}
    void goal_R();
    void preempt_R();
};
