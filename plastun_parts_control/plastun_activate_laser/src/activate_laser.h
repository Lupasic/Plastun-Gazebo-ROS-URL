#ifndef ACTIVATE_LASER_H
#define ACTIVATE_LASER_H

#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
//#include "../../../../devel/include/plastun_rotate_turret/angleAction.h"
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_activate_laser/FireAction.h"

class Activate_laser
{
protected:
    ros::NodeHandle nh;
    actionlib::SimpleActionServer<plastun_activate_laser::FireAction> *target;
    std::string action_name;
    plastun_activate_laser::FireResult result;
    plastun_activate_laser::FireGoal::ConstPtr goal;
    const static double eps = 0.02;

public:
    Activate_laser(std::string name);
    ~Activate_laser() {}
    void goal_R();
    void preempt_R();
    void start_laser();
};

#endif // ACTIVATE_LASER_H
