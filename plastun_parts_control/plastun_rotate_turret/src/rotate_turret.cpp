#include "rotate_turret.h"




Rotate_turret::Rotate_turret(std::string name):
    eps(0.02)
{
    action_name = name;
    rt_server = new  actionlib::SimpleActionServer<plastun_rotate_turret::angleAction>(nh,name,false);
    //register the goal and feeback callbacks
    rt_server->registerGoalCallback(boost::bind(&Rotate_turret::goal_R,this));
    rt_server->registerPreemptCallback(boost::bind(&Rotate_turret::preempt_R,this));
    // Add your ros communications here.
    turret_yaw_cur_pos = nh.subscribe("/plastun/turrel_revol_position_controller/state",1000,&Rotate_turret::turret_yaw_pos, this);
    turret_pitch_cur_pos = nh.subscribe("/plastun/turrel_up_to_down_position_controller/state",1000,&Rotate_turret::turret_pitch_pos, this);
    turret_yaw_command = nh.advertise<std_msgs::Float64>("/plastun/turrel_revol_position_controller/command",1000);
    turret_pitch_command = nh.advertise<std_msgs::Float64>("/plastun/turrel_up_to_down_position_controller/command",1000);
    fl_yaw = false;
    fl_pitch = false;

    rt_server->start();

}

void Rotate_turret::goal_R()
{

    goal = rt_server->acceptNewGoal();
    check.data = goal->alpha_yaw;
    analysis_yaw = goal->alpha_yaw;
    std::cout <<"Первоначальный угол по вертикальной оси: "<< feedback.cur_alpha_yaw << " по поперечной: "<< feedback.cur_alpha_pitch <<std::endl;
    std::cout <<"Угол поворота вокруг вертикальной оси: "<< goal->alpha_yaw << " вокруг поперечной оси: "<< goal->alpha_pitch <<std::endl;
    turret_yaw_command.publish(check);
    check.data = goal->alpha_pitch;
    analysis_pitch = goal->alpha_pitch;
    turret_pitch_command.publish(check);
    timer = nh.createTimer(ros::Duration(1), &Rotate_turret::timer_callback, this);
    timer.start();
}

void Rotate_turret::preempt_R()
{
    ROS_INFO("%s: Preempted", action_name.c_str());
    // set the action state to preempted
    rt_server->setPreempted();
}

void Rotate_turret::turret_yaw_pos(const control_msgs::JointControllerState &msg)
{
    if(fl_yaw == false)
    {
        feedback.cur_alpha_yaw = msg.process_value;
        std::cout << feedback.cur_alpha_yaw << std::endl;
        fl_yaw = true;
    }
    // make sure that the action hasn't been canceled
    if (!rt_server->isActive())
        return;
    feedback.cur_alpha_yaw = msg.process_value;


}

void Rotate_turret::turret_pitch_pos(const control_msgs::JointControllerState &msg)
{
    if(fl_pitch == false)
    {
        feedback.cur_alpha_pitch = msg.process_value;
        std::cout << feedback.cur_alpha_pitch << std::endl;
        fl_pitch = true;
    }
    // make sure that the action hasn't been canceled
    if (!rt_server->isActive())
        return;

    feedback.cur_alpha_pitch = msg.process_value;
    rt_server->publishFeedback(feedback);


}

void Rotate_turret::timer_callback(const ros::TimerEvent &event)
{
    std::cout <<std::abs(feedback.cur_alpha_yaw - analysis_yaw)<< "  тик  " <<std::abs(feedback.cur_alpha_pitch - analysis_pitch) <<std::endl;
    if((std::abs(feedback.cur_alpha_yaw - analysis_yaw)< eps) && (std::abs(feedback.cur_alpha_pitch - analysis_pitch)< eps))
    {
        plastun_rotate_turret::angleResult a;
        a.status = 1;
        rt_server->setSucceeded(a);
        timer.stop();
    }
}
