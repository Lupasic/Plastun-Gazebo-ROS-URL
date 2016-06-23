#include "activate_laser.h"
#define COUT(x) std::cout<<"Activate_laser:"<<x<<std::endl

Activate_laser::Activate_laser(std::string name)
{
    action_name = name;
    al_server = new  actionlib::SimpleActionServer<plastun_activate_laser::FireAction>(nh,name,false);
    //register the goal and feeback callbacks
    al_server->registerGoalCallback(boost::bind(&Activate_laser::goal_R,this));
    al_server->registerPreemptCallback(boost::bind(&Activate_laser::preempt_R,this));
    // Add your ros communications here.
    nh.getParam("/activate_laser/activate_topic", activate_topic);
    activ_laser = nh.subscribe(activate_topic, 1000,&Activate_laser::activate_laser_Callback, this);

    al_server->start();
    //инициализация лазера
    if (!initPorts())
    {
        COUT("can not init dio port");
    }

    setOut(7);
}

void Activate_laser::goal_R()
{
    goal = al_server->acceptNewGoal();
    start_laser();
}

void Activate_laser::preempt_R()
{
    ROS_INFO("%s: Preempted", action_name.c_str());
    // set the action state to preempted
    al_server->setPreempted();
}

void Activate_laser::start_laser()
{
    setLow(7);
    sleep(goal->fire_duration);
    setHigh(7);
    result.result =1;
    al_server->setSucceeded(result);
}

void Activate_laser::start_laser(int fire_duration)
{
    setLow(7);
    sleep(fire_duration);
    setHigh(7);
    std::cout << "лазер закончил работу" <<std::endl;
}

void Activate_laser::activate_laser_Callback(const std_msgs::Int32 &msg)
{
    start_laser(msg.data);
}

