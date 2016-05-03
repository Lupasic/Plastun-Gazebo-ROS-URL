#ifndef MOVE_CONTROL_H
#define MOVE_CONTROL_H

#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "geometry_msgs/PoseStamped.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_image_detect/access_detectAction.h"


class Move_control
{
protected:
    plastun_image_detect::access_detectGoal goal_access;
    actionlib::SimpleActionClient<plastun_image_detect::access_detectAction> *ac;
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> *mo;
    move_base_msgs::MoveBaseResult result_move;
    ros::Subscriber pr;
    move_base_msgs::MoveBaseActionGoal prov, prov_last;
    const static double eps = 0.1;

public:
    Move_control();
    void update();
    void goal_Callback(const move_base_msgs::MoveBaseActionGoal &msg);
    ~Move_control();
};

#endif // MOVE_CONTROL_H
