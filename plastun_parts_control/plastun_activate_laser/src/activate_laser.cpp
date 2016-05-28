#include "activate_laser.h"

Activate_laser::Activate_laser(std::string name)
{
    action_name = name;
    target = new  actionlib::SimpleActionServer<plastun_activate_laser::FireAction>(nh,name,false);
    //register the goal and feeback callbacks
    target->registerGoalCallback(boost::bind(&Activate_laser::goal_R,this));
    target->registerPreemptCallback(boost::bind(&Activate_laser::preempt_R,this));
    // Add your ros communications here.
    target->start();
}

void Activate_laser::goal_R()
{
    goal = target->acceptNewGoal();
    start_laser();
}

void Activate_laser::preempt_R()
{
    ROS_INFO("%s: Preempted", action_name.c_str());
    // set the action state to preempted
    target->setPreempted();
}

void Activate_laser::start_laser()
{
 //
}

