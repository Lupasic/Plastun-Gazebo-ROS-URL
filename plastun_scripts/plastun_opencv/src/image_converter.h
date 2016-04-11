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


class ImageConverter
{
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;
    ros::Publisher pub_point; // отправка топика
    std::vector<geometry_msgs::Pose> center;


public:
    ImageConverter();
    ~ImageConverter();
    void imageCb(const sensor_msgs::ImageConstPtr& msg);

};
