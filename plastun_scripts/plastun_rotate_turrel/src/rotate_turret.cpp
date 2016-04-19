#include "rotate_turret.h"
#include <cmath>

double x_x, y_y, im_x, im_y;
void target_pose_im_Callback(const geometry_msgs::Pose &msg)
{
  //  ROS_INFO("I heard: [%f]", msg.position.x);
    im_x = msg.position.x;
    im_y = msg.position.y;
}

void cur_pose_Callback(const tf2_msgs::TFMessage &msg)
{
    //ROS_INFO("TF: [%f]", msg.transforms[0].transform.translation.x);
    x_x = msg.transforms[0].transform.translation.x;
    y_y = msg.transforms[0].transform.translation.y;
}


Rotate_turret::Rotate_turret(double x, double y)
{
    target.position.x = x ;
    target.position.y = y ;
    ros::NodeHandle n;
    // Add your ros communications here.
    target_pose_im= n.subscribe("/image_converter/rectangle_center", 1000, target_pose_im_Callback);
    current_pose = n.subscribe("/tf",1000,cur_pose_Callback);


}

void Rotate_turret::rotate()
{

}

Rotate_turret::~Rotate_turret()
{
    if(ros::isStarted()) {
        ros::shutdown(); // explicitly needed since we use ros::start();
        ros::waitForShutdown();
    }
}

void Rotate_turret::update()
{
    im_target.position.x = im_x * 0.0026;
    im_target.position.y = im_y;
    cur_pose.position.x = x_x + 0.595;
    cur_pose.position.y = y_y + 0.21;
     std::cout <<"x " << cur_pose.position.x  <<"   y " << cur_pose.position.y << std::endl;
    Leng = std::sqrt(pow((cur_pose.position.x - target.position.x),2) + pow((cur_pose.position.y - target.position.y ),2));
    a_x = (std::asin(im_target.position.x/Leng));
    //std::cout <<"угол " << a_x/2 <<"   Длина " << Leng << std::endl;
    ros::spinOnce();
}

