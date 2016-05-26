#include "general_targeting.h"




General_targeting::General_targeting(std::string name)
{
    action_name = name;
    target = new  actionlib::SimpleActionServer<plastun_general_targeting::access_targetingAction>(nh,name,false);
    //register the goal and feeback callbacks
    target->registerGoalCallback(boost::bind(&General_targeting::goal_R,this));
    target->registerPreemptCallback(boost::bind(&General_targeting::preempt_R,this));
    // Add your ros communications here.
    target->start();

}

void General_targeting::goal_R()
{
   // goal = target->acceptNewGoal();
}

void General_targeting::preempt_R()
{
    ROS_INFO("%s: Preempted", action_name.c_str());
    // set the action state to preempted
    target->setPreempted();
}
