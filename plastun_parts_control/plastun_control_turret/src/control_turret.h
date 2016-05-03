#include <ros/ros.h>
#include "geometry_msgs/Pose.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/CameraInfo.h"
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include <tf2_msgs/TFMessage.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_rotate_turret/angleAction.h"

class Control_turret
{
protected:
    ros::NodeHandle nh;
    ros::Subscriber target_pose_im, current_pose, camera_info;
    ros::Publisher turret_camera_x, turret_camera_y;
    geometry_msgs::Pose target, im_target, cur_pose, im_target_last;
    std_msgs::Float64 angle_x,angle_y;
    float focal_length_x,focal_length_y, a_x, a_y;
    const static double eps = 0.1;
    actionlib::SimpleActionClient<plastun_rotate_turret::angleAction> *ac;
    plastun_rotate_turret::angleGoal goal;


public:
    Control_turret(double x, double y);
    ~Control_turret();
    void update();
    //void finishedCb(const actionlib::SimpleClientGoalState& state, const plastun_rotate_turret::angleResultPtr &result);
    void cur_pose_Callback(const tf2_msgs::TFMessage &msg);
    void target_pose_im_Callback(const geometry_msgs::Pose &msg);
    void camera_info_Callback(const sensor_msgs::CameraInfo &msg);

};
