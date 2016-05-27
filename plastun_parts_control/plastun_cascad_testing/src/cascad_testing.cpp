#include "cascad_testing.h"



static const std::string OPENCV_WINDOW = "Image window";


CascadTesting::CascadTesting()
    : it_(nh_)
{
//Получаем топик для камеры из launch файла
        std::string suscribe_camera_topic;
        nh_.getParam("/cascad_testing/suscribe_camera_topic",suscribe_camera_topic);
    // Subscrive to input video feed and publish output video feed

    image_sub_ = it_.subscribe(suscribe_camera_topic, 1,
                               &CascadTesting::imageCb, this);
    image_pub_ = it_.advertise("/cascad_testing/output_video", 1);
    pub_point = nh_.advertise<geometry_msgs::Pose>("/cascad_testing/rectangle_center",1000);

    cv::namedWindow(OPENCV_WINDOW);
}

CascadTesting::~CascadTesting()
{
    cv::destroyWindow(OPENCV_WINDOW);
}

void CascadTesting::imageCb(const sensor_msgs::ImageConstPtr& msg)
{
	//Получаем xml с каскадом из launch файла
        std::string cascad;
        nh_.getParam("/cascad_testing/cascad",cascad);

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

    cv::Mat gray;
    cv::Point centar(cv_ptr->image.cols/2, cv_ptr->image.rows/2);
    cvtColor(cv_ptr->image, gray, cv::COLOR_BGR2GRAY); // Перевод в чёрно-белое
    cv::CascadeClassifier cascadeSymbol; // Объявление каскада
    bool cascadeSymbolLoad = cascadeSymbol.load(cascad); // Загрузка каскада

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
   // cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
    pub_point.publish(center[0]);
    center.clear();
}
