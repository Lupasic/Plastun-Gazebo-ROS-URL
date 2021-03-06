#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect//objdetect.hpp"
#include <algorithm>
#include <iostream>
#include <math.h>
#include "geometry_msgs/Pose.h"
#include <tf/transform_listener.h>
#include <string>
#include <vector>
#include <actionlib/server/simple_action_server.h>
#include <plastun_image_detect/access_detectAction.h>


class Image_detect
{
protected:
    ros::NodeHandle nh_;
    std::string action_name;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;
    ros::Publisher pub_point; // отправка топика
    std::vector<geometry_msgs::Pose> center;
    actionlib::SimpleActionServer<plastun_image_detect::access_detectAction> *id_server;
    plastun_image_detect::access_detectFeedback feedback;
    plastun_image_detect::access_detectResult result;
    plastun_image_detect::access_detectGoal::ConstPtr goal;
    bool fl;


public:
    Image_detect(std::string name);
    ~Image_detect();
    void goal_R();
    void preempt_R();
    void imageCallback(const sensor_msgs::ImageConstPtr& msg);

};
