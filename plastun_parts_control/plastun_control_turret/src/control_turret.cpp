#include "control_turret.h"
#include <cmath>

void Control_turret::target_pose_im_Callback(const geometry_msgs::Pose &msg)
{
    //  ROS_INFO("I heard: [%f]", msg.position.x);
    im_target.position.x = msg.position.x;
    im_target.position.y = msg.position.y;
}

void Control_turret::camera_info_Callback(const sensor_msgs::CameraInfo &msg)
{
    focal_length_x = msg.K[0];
    focal_length_y = msg.K[4];
}

void Control_turret::cur_pose_Callback(const tf2_msgs::TFMessage &msg)
{
    //ROS_INFO("TF: [%f]", msg.transforms[0].transform.translation.x);
    cur_pose.position.x = msg.transforms[0].transform.translation.x;
    cur_pose.position.y = msg.transforms[0].transform.translation.y;
}


Control_turret::Control_turret(double x, double y)
{
    target.position.x = x ;
    target.position.y = y ;
    im_target_last.position.x = 0;
    im_target_last.position.y = 0;
    ros::NodeHandle n;
    // Add your ros communications here.
    target_pose_im= n.subscribe("/image_converter/rectangle_center", 1000, &Control_turret::target_pose_im_Callback,this);
    current_pose = n.subscribe("/tf",1000,&Control_turret::cur_pose_Callback, this);
    camera_info = n.subscribe("/plastun/camera_1/camera_info_1",1000,&Control_turret::camera_info_Callback, this);
    ac = new actionlib::SimpleActionClient<plastun_rotate_turret::angleAction>("rotate_turret", true);

}

Control_turret::~Control_turret()
{
    if(ros::isStarted()) {
        ros::shutdown(); // explicitly needed since we use ros::start();
        ros::waitForShutdown();
    }
}

//void Control_turret::finishedCb(const actionlib::SimpleClientGoalState &state, const plastun_rotate_turret::angleResultPtr &result)
//{
//    //ROS_INFO("Finished in state [%s]", state.toString().c_str());
//    //ROS_INFO("Answer: %d", result->status.);
//}

void Control_turret::update()
{

    if((im_target_last.position.x != im_target.position.x) || (im_target_last.position.y != im_target.position.y))
    {
        im_target_last= im_target;
        a_x = std::atan(im_target.position.x/focal_length_x);
        a_y = std::atan(im_target.position.y/focal_length_y);

        goal.alpha_x = a_x;
        goal.alpha_y = a_y;
        ROS_INFO("Waiting for action server to start.");
        // wait for the action server to start
        ac->waitForServer(); //will wait for infinite time

        ROS_INFO("Action server started, sending goal.");
        // send a goal to the action
        std::cout <<"угол x " << a_x <<"   Угол y " << a_y << std::endl;
        //ac->sendGoal(goal, boost::bind(&Control_turret::finishedCb, this ,_1,_2));
        ac->sendGoal(goal);
        if(ac->waitForResult())
        {
            //int a = ac->getResult()->status;
            //std::cout << a << std::endl;
            //Тут будет включения лазера для железа
        }
    }
    ros::spinOnce();
}



