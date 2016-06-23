#ifndef ACTIVATE_LASER_H
#define ACTIVATE_LASER_H

#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <plastun_activate_laser/FireAction.h>
#include <std_msgs/Int32.h>
#include "dioAurora/dioport.h"
#include "dioAurora/ecram.h"

class Activate_laser
{
protected:
    ros::NodeHandle nh;
    ros::Subscriber activ_laser;
    actionlib::SimpleActionServer<plastun_activate_laser::FireAction> *al_server;
    std::string action_name, activate_topic;
    plastun_activate_laser::FireResult result;
    plastun_activate_laser::FireGoal::ConstPtr goal;
    const static double eps = 0.02;

public:
    Activate_laser(std::string name);
    ~Activate_laser() {}
    void goal_R();
    void preempt_R();
    void start_laser();
    void start_laser(int fire_duration);
    //Колбеки
    void activate_laser_Callback(const std_msgs::Int32 &msg);
};

#endif // ACTIVATE_LASER_H
