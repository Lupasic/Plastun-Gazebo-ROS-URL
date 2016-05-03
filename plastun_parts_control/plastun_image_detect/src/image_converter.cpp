#include "image_converter.h"



static const std::string OPENCV_WINDOW = "Image window";


ImageConverter::ImageConverter(std::string name)
    : it_(nh_)
{
    action_name = name;
    //ros::NodeHandle n;
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/plastun/camera_1/image_raw_1", 1,
                               &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);
    pub_point = nh_.advertise<geometry_msgs::Pose>("/image_converter/rectangle_center",1000);
    angl = new actionlib::SimpleActionServer<plastun_image_detect::access_detectAction>(nh_,name,false);
    angl->registerGoalCallback(boost::bind(&ImageConverter::goal_R,this));
    angl->registerPreemptCallback(boost::bind(&ImageConverter::preempt_R,this));
    cv::namedWindow(OPENCV_WINDOW);
    fl = false;

    angl->start();
}

ImageConverter::~ImageConverter()
{
    //cv::destroyWindow(OPENCV_WINDOW);
}

void ImageConverter::goal_R()
{
    int a;
    goal = angl->acceptNewGoal();
    a = goal->access;
    if(a == 2)
        fl = true;
}

void ImageConverter::preempt_R()
{
    ROS_INFO("%s: Preempted", action_name.c_str());
    // set the action state to preempted
    angl->setPreempted();
}

void ImageConverter::imageCb(const sensor_msgs::ImageConstPtr& msg)
{
    if(fl == true)
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

    //Draw an example circle on the video stream
    cv::circle(cv_ptr->image, cv::Point(512.5, 384.5), 5, CV_RGB(255,0,0));
    cv::Mat gray;
    cv::Point centar(cv_ptr->image.cols/2, cv_ptr->image.rows/2);
    cvtColor(cv_ptr->image, gray, cv::COLOR_BGR2GRAY); // Перевод в чёрно-белое
    cv::CascadeClassifier cascadeSymbol; // Объявление каскада
    bool cascadeSymbolLoad = cascadeSymbol.load("/home/lupasic/Programs/catkin_ws/src/plastun_gazebo/plastun_parts_control/plastun_image_detect/cascads/plafon_gazebo/cascade.xml"); // Загрузка каскада

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
        //std::cout << centar.x <<" " << centar.y << std::endl;
        //std::cout << "X: " << p.x << " Y: " << p.y << " Width: " << p.width << " Height: " << p.height << std::endl;
        a.position.x=centar.x-(p.x + p.width/2);
        a.position.y=centar.y-(p.y + p.height/2);
        center.push_back(a);
        std::cout << "X_smesh: " << a.position.x << " Y_smesh: " << a.position.y << std::endl;

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
    fl = false;
    }
}
