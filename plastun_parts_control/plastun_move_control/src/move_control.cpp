#include "move_control.h"

Move_control::Move_control()
{
    ros::NodeHandle n;
    ac = new actionlib::SimpleActionClient<plastun_image_detect::access_detectAction>("image_converter", true);
    mo = new actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>("move_base", true);
    pr = n.subscribe("/move_base/goal",1000,&Move_control::goal_Callback, this);
}

void Move_control::update()
{
    if(( prov.goal.target_pose.pose.position.x - prov_last.goal.target_pose.pose.position.x) > eps
            || ( prov.goal.target_pose.pose.position.y - prov_last.goal.target_pose.pose.position.y) > eps)
    {
        prov_last =prov;
        mo->sendGoal(prov.goal);
        if(mo->waitForResult())
        {
            std::cout << " Ждем сервера" << std::endl;
            goal_access.access = 2;
            ac->waitForServer();
            std::cout << " Пересылаем" << std::endl;
            ac->sendGoal(goal_access);
        }
    }

    ros::spinOnce();
}

void Move_control::goal_Callback(const move_base_msgs::MoveBaseActionGoal &msg)
{
    prov = msg;

}

Move_control::~Move_control()
{
    if(ros::isStarted()) {
        ros::shutdown(); // explicitly needed since we use ros::start();
        ros::waitForShutdown();
    }
}

