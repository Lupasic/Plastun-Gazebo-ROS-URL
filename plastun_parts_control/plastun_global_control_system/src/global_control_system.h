#ifndef GLOBAL_CONTROL_SYSTEM_H
#define GLOBAL_CONTROL_SYSTEM_H

#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "geometry_msgs/PoseStamped.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_image_detect/access_detectAction.h"
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_general_targeting/access_targetingAction.h"
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_rotate_turret/angleAction.h"
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_activate_laser/FireAction.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/CameraInfo.h"
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include <tf2_msgs/TFMessage.h>
#include "geometry_msgs/Pose.h"
#include <cmath>

class Global_control_system
{
protected:
    //клиенты
    actionlib::SimpleActionClient<plastun_image_detect::access_detectAction> *id;
    actionlib::SimpleActionClient<plastun_general_targeting::access_targetingAction> *gt;
    actionlib::SimpleActionClient<plastun_rotate_turret::angleAction> *rt;
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> *mb;
    actionlib::SimpleActionClient<plastun_activate_laser::FireAction> *al;
    //Сускрайберы
    ros::Subscriber pr, camera_info, clicked_point;
    //Погрешность
    const static double eps = 0.1;
    //Goals
    plastun_image_detect::access_detectGoal goal_access;
    plastun_general_targeting::access_targetingGoal goal_targeting;
    plastun_rotate_turret::angleGoal goal_rotate;
    plastun_activate_laser::FireGoal goal_laser;
    move_base_msgs::MoveBaseGoal prov;
    //Функции при завершении работы сервера
    void move_base_finishedCb(const actionlib::SimpleClientGoalState& state);
    void general_targeting_finishedCb(const actionlib::SimpleClientGoalState& state, const plastun_general_targeting::access_targetingResultConstPtr &result);
    void image_detect_finishedCb(const actionlib::SimpleClientGoalState& state, const plastun_image_detect::access_detectResultConstPtr &result);
    void rotate_turret_finishedCb(const actionlib::SimpleClientGoalState& state, const plastun_rotate_turret::angleResultConstPtr &result);
    void activate_laser_finishedCb(const actionlib::SimpleClientGoalState& state);
    //
    float focal_length_x,focal_length_y, a_x, a_y;
    std_msgs::Float64 angle_x,angle_y;
    geometry_msgs::Point cur_target;
    int x_sm, y_sm;
    bool fl_rotate_status, fl_camera_info, fl_first_rotate;
    //Вспомогательные функции
    void angle_count();
    void move_base_sending_goal();


public:
    Global_control_system();
    ~Global_control_system();
    //Колбеки
    void goal_Callback(const geometry_msgs::PoseStamped &msg);
    void target_pose_im_Callback(const geometry_msgs::Pose &msg);
    void camera_info_Callback(const sensor_msgs::CameraInfo &msg);
    void target_points_Callback(const geometry_msgs::PointStamped &msg);

};

#endif // GLOBAL_CONTROL_SYSTEM_H
