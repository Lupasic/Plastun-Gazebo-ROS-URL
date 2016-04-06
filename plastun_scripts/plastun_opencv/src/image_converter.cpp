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


static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/plastun/camera_2/image_raw_2", 1,
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    // Draw an example circle on the video stream
//    if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
//      cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));
    cv::Mat gray;
    cvtColor(cv_ptr->image, gray, cv::COLOR_BGR2GRAY); // Перевод в чёрно-белое
    cv::CascadeClassifier cascadeSymbol; // Объявление каскада
    bool cascadeSymbolLoad = cascadeSymbol.load("/home/lupasic/Programs/Learn/gazebo_plafon/plafon_gazebo/cascade.xml"); // Загрузка каскада

    if(!cascadeSymbolLoad)
    {
        std::cerr << "Cascade not load. Check your directory \"haarcascade_russian_plate_number_symbol.xml\"" << std::endl;
    }

    std::vector<cv::Rect> symbols;
    cascadeSymbol.detectMultiScale(gray, symbols); // Поиск с помощью каскада

    for(auto& p : symbols)
    {
        cv::Point symbolBegin	= cv::Point(p.x, p.y);
        cv::Point symbolEnd		= cv::Point(p.x+p.width, p.y+p.height);

        std::cout << "X: " << p.x << " Y: " << p.y << " Width: " << p.width << " Height: " << p.height << std::endl;


        rectangle(cv_ptr->image, symbolBegin, symbolEnd, cv::Scalar(0,255,0), 2);
        break; //Для Каскада газебо
    }


    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
