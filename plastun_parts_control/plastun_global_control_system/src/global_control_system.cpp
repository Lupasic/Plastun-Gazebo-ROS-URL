#include "global_control_system.h"

//Конструктор
Global_control_system::Global_control_system()
{
    ros::NodeHandle n;
    //Action clients
    mb = new actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>("move_base", false);
    id = new actionlib::SimpleActionClient<plastun_image_detect::access_detectAction>("image_detect", false);
    rt = new actionlib::SimpleActionClient<plastun_rotate_turret::angleAction>("rotate_turret", false);
    //Читаем топики
    pr = n.subscribe("/move_base_simple1/goal",1000,&Global_control_system::goal_Callback, this); //Чтение идет из Rviz, с замененым там топиком, чтобы не слал его сразу move_base -у
    current_pose = n.subscribe("/tf",1000,&Global_control_system::cur_pose_Callback, this);
    camera_info = n.subscribe("/plastun/camera_1/camera_info_1",1000,&Global_control_system::camera_info_Callback, this);

    //Ждем сервера
    while(!mb->isServerConnected() && !id->isServerConnected() && !rt->isServerConnected())
        ros::spinOnce();
    //Задаем флаги
    fl_rotate_status = false;
    fl_camera_info = false;
}
//Деструктор
Global_control_system::~Global_control_system()
{
    if(ros::isStarted()) {
        ros::shutdown(); // explicitly needed since we use ros::start();
        ros::waitForShutdown();
    }
}

//Чтение топиков
void Global_control_system::goal_Callback(const geometry_msgs::PoseStamped &msg)
{
    prov.target_pose = msg;
    this->move_base_sending_goal(); //Как пришел топик, отпраляем actionGoal
}



void Global_control_system::camera_info_Callback(const sensor_msgs::CameraInfo &msg)
{
    if(fl_camera_info == false)
    {
        focal_length_x = msg.K[0];
        focal_length_y = msg.K[4];
        fl_camera_info = true;
    }
}

void Global_control_system::cur_pose_Callback(const tf2_msgs::TFMessage &msg)
{
    //ROS_INFO("TF: [%f]", msg.transforms[0].transform.translation.x);
    cur_pose.position.x = msg.transforms[0].transform.translation.x;
    cur_pose.position.y = msg.transforms[0].transform.translation.y;
}



//Реализация функции при завершении работы сервера(основная логика)
void Global_control_system::move_base_finishedCb(const actionlib::SimpleClientGoalState &state)
{
    ROS_INFO("Robot in position status: [%s]", state.toString().c_str());
    goal_access.access = 2;
    id->sendGoal(goal_access, boost::bind(&Global_control_system::image_detect_finishedCb, this ,_1,_2));
}


void Global_control_system::image_detect_finishedCb(const actionlib::SimpleClientGoalState &state, const plastun_image_detect::access_detectResultConstPtr &result)
{
    if(result->result == true)
    {
        ROS_INFO("Image_detect status: [%s]", state.toString().c_str());
        x_sm = result->x_smesh;
        y_sm = result->y_smesh;
        std::cout << "Смещение от центра по 'x': " << x_sm <<" по 'y': " << y_sm << std::endl;
        this->angle_count();

        rt->sendGoal(goal_rotate, boost::bind(&Global_control_system::rotate_turret_finishedCb, this ,_1,_2),
                                  actionlib::SimpleActionClient<plastun_rotate_turret::angleAction>::SimpleActiveCallback(),
                                  boost::bind(&Global_control_system::rotate_turret_feedbackCb, this, _1));
        fl_rotate_status = true;
    }
    else
        ROS_INFO("No image detected.");
}

void Global_control_system::rotate_turret_finishedCb(const actionlib::SimpleClientGoalState &state, const plastun_rotate_turret::angleResultConstPtr &result)
{
    ROS_INFO("Rotate turret status: [%s]", state.toString().c_str());
    //Лазер
}
//Реализация функции приема фидбека
void Global_control_system::rotate_turret_feedbackCb(const plastun_rotate_turret::angleFeedbackConstPtr &feedback)
{
    feedback_rotate = feedback;
}


void Global_control_system::move_base_sending_goal()
{
    ROS_INFO("Moving robot in new position");

    mb->sendGoal(prov, boost::bind(&Global_control_system::move_base_finishedCb, this ,_1));
    if(fl_rotate_status == true)
    {

        //rt->cancelGoal();
        goal_rotate.alpha_x = -feedback_rotate->cur_alpha_x;
        goal_rotate.alpha_y = -feedback_rotate->cur_alpha_y;
        rt->sendGoal(goal_rotate, boost::bind(&Global_control_system::rotate_turret_finishedCb, this ,_1,_2));
        fl_rotate_status = false;
    }
}



void Global_control_system::angle_count()
{
    a_x = std::atan(x_sm/focal_length_x);
    a_y = std::atan(y_sm/focal_length_y);
    goal_rotate.alpha_x = a_x;
    goal_rotate.alpha_y = a_y;
}
