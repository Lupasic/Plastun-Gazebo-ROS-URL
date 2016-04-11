#include "image_converter.h"



static const std::string OPENCV_WINDOW = "Image window";


ImageConverter::ImageConverter()
    : it_(nh_)
{
    //ros::NodeHandle n;
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/plastun/camera_1/image_raw_1", 1,
                               &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);
    pub_point = nh_.advertise<geometry_msgs::Pose>("/image_converter/rectangle_center",1000);

    cv::namedWindow(OPENCV_WINDOW);
}

ImageConverter::~ImageConverter()
{
    cv::destroyWindow(OPENCV_WINDOW);
}

void ImageConverter::imageCb(const sensor_msgs::ImageConstPtr& msg)
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
    bool cascadeSymbolLoad = cascadeSymbol.load("/home/lupasic/Programs/catkin_ws/src/plastun_gazebo/plastun_scripts/plastun_opencv/cascads/plafon_gazebo/cascade.xml"); // Загрузка каскада

    if(!cascadeSymbolLoad)
    {
        std::cerr << "Cascade not load. Check your directory \"haarcascade_russian_plate_number_symbol.xml\"" << std::endl;
    }

    std::vector<cv::Rect> symbols;
    cascadeSymbol.detectMultiScale(gray, symbols); // Поиск с помощью каскада
    geometry_msgs::Pose a;
    for(auto& p : symbols)
    {
        cv::Point symbolBegin	= cv::Point(p.x, p.y);
        cv::Point symbolEnd		= cv::Point(p.x+p.width, p.y+p.height);

        std::cout << "X: " << p.x << " Y: " << p.y << " Width: " << p.width << " Height: " << p.height << std::endl;
        a.position.x=p.x;
        a.position.y=p.y;
        center.push_back(a);

        rectangle(cv_ptr->image, symbolBegin, symbolEnd, cv::Scalar(0,255,0), 2);
        break; //Для Каскада газебо
    }


    // Update GUI Window
    //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
    pub_point.publish(center[0]);
    center.clear();
}
