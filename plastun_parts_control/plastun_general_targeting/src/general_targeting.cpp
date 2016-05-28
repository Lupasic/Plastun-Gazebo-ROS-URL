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
    listener = new tf::TransformListener();
}

void General_targeting::goal_R()
{
    goal = target->acceptNewGoal();
    main_calculation();
}

void General_targeting::preempt_R()
{
    ROS_INFO("%s: Preempted", action_name.c_str());
    // set the action state to preempted
    target->setPreempted();
}

void General_targeting::main_calculation()
{
    tf::StampedTransform transform;
    ros::Rate rate(10.0);
    try{
        listener->lookupTransform("/map", "/turrel_vniz",
                                  ros::Time(0), transform);

    }
    catch (tf::TransformException &ex)
    {
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
    }
    cur_pos_x = transform.getOrigin().x();
    cur_pos_y = transform.getOrigin().y();
    lengh_to_cam_xy = 0.21;
    lengh_to_target = sqrt(pow((goal->target_x - cur_pos_x),2) + pow((goal->target_y - cur_pos_y),2));
    //1.57 это 90 град
    result.angle_x = acos(lengh_to_cam_xy/lengh_to_target) - 1.57 + asin(abs(goal->target_y - cur_pos_y)/lengh_to_target);
    if((goal->target_y - cur_pos_y) < 0)
    {
        float a = result.angle_x;
        result.angle_x = -a;
    }
    //result.angle_y = acos(lengh_to_cam_yz/lengh_to_target) - 1.57 + asin((goal->target_y - cur_pos_y)/lengh_to_target);
    result.angle_y = 0;
    target->setSucceeded(result);
}
