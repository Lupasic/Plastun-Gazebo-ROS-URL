#ifndef GLOBAL_CONTROL_SYSTEM_H
#define GLOBAL_CONTROL_SYSTEM_H

#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "geometry_msgs/PoseStamped.h"
#include "move_base_msgs/MoveBaseAction.h"
#include <plastun_image_detect/access_detectAction.h>
#include <plastun_general_targeting/access_targetingAction.h>
#include <plastun_rotate_turret/angleAction.h>
#include <plastun_activate_laser/FireAction.h>
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
    actionlib::SimpleActionClient<plastun_image_detect::access_detectAction> *id; //image detect
    actionlib::SimpleActionClient<plastun_general_targeting::access_targetingAction> *gt; //general targeting
    actionlib::SimpleActionClient<plastun_rotate_turret::angleAction> *rt; //rotate turret
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> *mb; //move base
    actionlib::SimpleActionClient<plastun_activate_laser::FireAction> *al; //activate laser
    //Сускрайберы
    ros::Subscriber pr, camera_info, clicked_point;
    //Goals
    plastun_image_detect::access_detectGoal goal_access;  //image detect, посылает статус разрешения
    plastun_general_targeting::access_targetingGoal goal_targeting; //general targeting, посылает координаты цели
    plastun_rotate_turret::angleGoal goal_rotate; //rotate turret, посылает углы поворота
    plastun_activate_laser::FireGoal goal_laser; //activate laser, активирует лазер
    move_base_msgs::MoveBaseGoal prov;  //move base, посылает координаты цели
    //Функции при завершении работы сервера
    void move_base_finishedCb(const actionlib::SimpleClientGoalState& state);
    void general_targeting_finishedCb(const actionlib::SimpleClientGoalState& state, const plastun_general_targeting::access_targetingResultConstPtr &result);
    void image_detect_finishedCb(const actionlib::SimpleClientGoalState& state, const plastun_image_detect::access_detectResultConstPtr &result);
    void rotate_turret_finishedCb(const actionlib::SimpleClientGoalState& state, const plastun_rotate_turret::angleResultConstPtr &result);
    void activate_laser_finishedCb(const actionlib::SimpleClientGoalState& state);
    //
    ros::NodeHandle n;
    float focal_length_x,focal_length_y, a_yaw, a_pitch;
    float firstly_yaw, firstly_pitch;
    std_msgs::Float64 angle_yaw,angle_pitch;
    geometry_msgs::Point cur_target;
    int x_sm, y_sm, sleep_targeting_duration;
    bool fl_rotate_status, fl_camera_info, fl_first_rotate; //1 - за возврат туррели, 2 - чтобы самера инфо считалась 1 раз и все, 3 - работает 2 раза, разбиение логики
    std::string camera_info_topic, target_points_topic;
    //Вспомогательные функции
    void angle_count(); //пересчет смещения на изоюражении в углы
    void move_base_sending_goal();  //начало конечного автомата


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
