#include "image_detect.h"



static const std::string OPENCV_WINDOW = "Image window";


Image_detect::Image_detect(std::string name)
    : it_(nh_)
{
    action_name = name;
    //Получаем топик для камеры из launch файла
    std::string suscribe_camera_topic;
    nh_.getParam("/cascad_testing/suscribe_camera_topic",suscribe_camera_topic);
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe(suscribe_camera_topic, 1, &Image_detect::imageCallback, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);
    id_server = new actionlib::SimpleActionServer<plastun_image_detect::access_detectAction>(nh_,name,false);
    id_server->registerGoalCallback(boost::bind(&Image_detect::goal_R,this));
    id_server->registerPreemptCallback(boost::bind(&Image_detect::preempt_R,this));
    cv::namedWindow(OPENCV_WINDOW);

    fl = false;
    id_server->start();
}

Image_detect::~Image_detect()
{
    //cv::destroyWindow(OPENCV_WINDOW);
}

void Image_detect::goal_R()
{
    int a;
    goal = id_server->acceptNewGoal();
    a = goal->access;
    if(a == 2)
        fl = true;
}

void Image_detect::preempt_R()
{
    ROS_INFO("%s: Preempted", action_name.c_str());
    // set the action state to preempted
    id_server->setPreempted();
    fl = false;
}

void Image_detect::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{

    if(fl == true)
    {
        //Получаем xml с каскадом из launch файла
        std::string cascad;
        nh_.getParam("/image_detect/cascad",cascad);

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
        bool cascadeSymbolLoad = cascadeSymbol.load(cascad); // Загрузка каскада

        if(!cascadeSymbolLoad)
        {
            std::cerr << "Cascade not load. Check your directory in launch file" << std::endl;
        }
        std::vector<cv::Rect> symbols;
        cascadeSymbol.detectMultiScale(gray, symbols); // Поиск с помощью каскада
        geometry_msgs::Pose a;
        for(auto& p : symbols)
        {
            cv::Point symbolBegin = cv::Point(p.x, p.y);
            cv::Point symbolEnd = cv::Point(p.x+p.width, p.y+p.height);
            a.position.x=centar.x-(p.x + p.width/2);
            a.position.y=centar.y-(p.y + p.height/2);
            center.push_back(a);
            //для дебага центра
            rectangle(cv_ptr->image, symbolBegin, symbolEnd, cv::Scalar(0,255,0), 2);
            break; //Для Каскада газебо
        }

        plastun_image_detect::access_detectResult res;
        if(!center.empty())
        {

            res.result = true;
            res.x_smesh = center[0].position.x;
            res.y_smesh = center[0].position.y;
            id_server->setSucceeded(res);
            image_pub_.publish(cv_ptr->toImageMsg());
            center.clear();
        }
        else
        {
            res.result = false;
            id_server->setSucceeded(res);
        }

        fl = false;
    }
}
