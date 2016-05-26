#include "rotate_turret.h"




Rotate_turret::Rotate_turret(std::string name)
{
    action_name = name;
    angl = new  actionlib::SimpleActionServer<plastun_rotate_turret::angleAction>(nh,name,false);
    //register the goal and feeback callbacks
    angl->registerGoalCallback(boost::bind(&Rotate_turret::goal_R,this));
    angl->registerPreemptCallback(boost::bind(&Rotate_turret::preempt_R,this));
    // Add your ros communications here.
//    turret_x_cur_pos = nh.subscribe("/plastun/turrel_revol_position_controller/state",1000,&Rotate_turret::turret_x_pos, this);
//    turret_y_cur_pos = nh.subscribe("/plastun/turrel_up_to_down_position_controller/state",1000,&Rotate_turret::turret_y_pos, this);
//    turret_x_command = nh.advertise<std_msgs::Float64>("/plastun/turrel_revol_position_controller/command",1000);
//    turret_y_command = nh.advertise<std_msgs::Float64>("/plastun/turrel_up_to_down_position_controller/command",1000);

    fl_x = false;
    fl_y = false;
    angl->start();

}

void Rotate_turret::goal_R()
{
    goal = angl->acceptNewGoal();
//    check.data = goal->alpha_x + feedback.cur_alpha_x;
//    std::cout <<"Первоначальный угол по 'x': "<< feedback.cur_alpha_x << " по 'y': "<< feedback.cur_alpha_y <<std::endl;
//    std::cout <<"Угол поворота по 'x': "<< goal->alpha_x << " по 'y': "<< goal->alpha_y <<std::endl;
//    turret_x_command.publish(check);
//    check.data = goal->alpha_y + feedback.cur_alpha_y;
//    turret_y_command.publish(check);
}

void Rotate_turret::preempt_R()
{
    ROS_INFO("%s: Preempted", action_name.c_str());
    // set the action state to preempted
    angl->setPreempted();
}

//void Rotate_turret::turret_x_pos(const control_msgs::JointControllerState &msg)
//{
//    if(fl_x == false)
//    {
//        feedback.cur_alpha_x = msg.process_value;
//        std::cout << feedback.cur_alpha_x << std::endl;
//        fl_x = true;
//    }
//    // make sure that the action hasn't been canceled
//    if (!angl->isActive())
//      return;

//    feedback.cur_alpha_x = msg.process_value;
//}

//void Rotate_turret::turret_y_pos(const control_msgs::JointControllerState &msg)
//{
//    if(fl_y == false)
//    {
//        feedback.cur_alpha_y = msg.process_value;
//        std::cout << feedback.cur_alpha_y << std::endl;
//        fl_y = true;
//    }
//    // make sure that the action hasn't been canceled
//    if (!angl->isActive())
//      return;

//    feedback.cur_alpha_y = msg.process_value;
//    angl->publishFeedback(feedback);
//    if(( feedback.cur_alpha_x - goal->alpha_x)< eps && (feedback.cur_alpha_y -  goal->alpha_y)< eps)
//    {
//        plastun_rotate_turret::angleResult a;
//        a.status = 1;
//        angl->setSucceeded(a);
//    }
//}
