#include "global_control_system.h"

//Конструктор
Global_control_system::Global_control_system()
{
    ros::NodeHandle n;
    //Action clients
    mb = new actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>("move_base", false);
    id = new actionlib::SimpleActionClient<plastun_image_detect::access_detectAction>("image_detect", false);
    gt = new actionlib::SimpleActionClient<plastun_general_targeting::access_targetingAction>("general_targeting", false);
    rt = new actionlib::SimpleActionClient<plastun_rotate_turret::angleAction>("rotate_turret", false);
    //Читаем топики
    pr = n.subscribe("/move_base_simple1/goal",1000,&Global_control_system::goal_Callback, this); //Чтение идет из Rviz, с замененым там топиком, чтобы не слал его сразу move_base -у
    camera_info = n.subscribe("/plastun/camera_1/camera_info_1",1000,&Global_control_system::camera_info_Callback, this); //снятие информации о камере
    clicked_point = n.subscribe("/target_points",1000 ,&Global_control_system::target_points_Callback, this);//Чтение координат Published point
    //Ждем сервера
    while(!mb->isServerConnected() && !id->isServerConnected() && !rt->isServerConnected() && !gt->isServerConnected())
        ros::spinOnce();
    //Задаем флаги
    fl_rotate_status = false;
    fl_camera_info = false;
    fl_first_rotate = true;
    //Проверка на присутствие координаты цели
    cur_target.x = 0;
    cur_target.y = 0;
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

void Global_control_system::target_points_Callback(const geometry_msgs::PointStamped &msg)
{
    cur_target = msg.point;
}


//Реализация функции при завершении работы сервера(основная логика)
void Global_control_system::move_base_finishedCb(const actionlib::SimpleClientGoalState &state)
{
    ROS_INFO("Robot in position status: [%s]", state.toString().c_str());
    if(cur_target.x != 0 || cur_target.y != 0) //Проверка на присутствие координат цели
    {
        goal_targeting.target_x = cur_target.x;
        goal_targeting.target_y = cur_target.y;
        gt->sendGoal(goal_targeting, boost::bind(&Global_control_system::general_targeting_finishedCb, this ,_1,_2));
    }
    else
    {
        ROS_INFO("Need to give a target, abort command");
    }
}

void Global_control_system::general_targeting_finishedCb(const actionlib::SimpleClientGoalState &state, const plastun_general_targeting::access_targetingResultConstPtr &result)
{
    ROS_INFO("General_targeting status: [%s]", state.toString().c_str());
    std::cout << "Нужно повернуть туррель вокруг вертикальной оси: " << result->angle_yaw <<" вокруг поперечной оси: " << result->angle_pitch << std::endl;
    goal_rotate.alpha_yaw = result->angle_yaw;
    goal_rotate.alpha_pitch = result->angle_pitch;
    fl_first_rotate = true;
    rt->sendGoal(goal_rotate, boost::bind(&Global_control_system::rotate_turret_finishedCb, this ,_1,_2));
}

void Global_control_system::image_detect_finishedCb(const actionlib::SimpleClientGoalState &state, const plastun_image_detect::access_detectResultConstPtr &result)
{
    fl_rotate_status = true;
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
    }
    else
    {
        ROS_INFO("No image detected.");
        //Вернуть турель в начальное положение
        goal_rotate.alpha_yaw = -feedback_rotate->cur_alpha_yaw;
        goal_rotate.alpha_pitch = -feedback_rotate->cur_alpha_pitch;
        rt->sendGoal(goal_rotate, boost::bind(&Global_control_system::rotate_turret_finishedCb, this ,_1,_2));

    }
}

void Global_control_system::rotate_turret_finishedCb(const actionlib::SimpleClientGoalState &state, const plastun_rotate_turret::angleResultConstPtr &result)
{
    ROS_INFO("Rotate turret status: [%s]", state.toString().c_str());
    if(fl_first_rotate == true)
    {
        std::cout << "Активируем донаведение " << std::endl;
        goal_access.access = 2;
        fl_first_rotate = false;
        //id->sendGoal(goal_access, boost::bind(&Global_control_system::image_detect_finishedCb, this ,_1,_2));
    }
    else
    {
        std::cout << "Поздравляю, цель поражена" << std::endl;
        fl_rotate_status = true;
    }

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
        //Вернуть турель в начальное положение
        goal_rotate.alpha_yaw = -feedback_rotate->cur_alpha_yaw;
        goal_rotate.alpha_pitch = -feedback_rotate->cur_alpha_pitch;
        rt->sendGoal(goal_rotate, boost::bind(&Global_control_system::rotate_turret_finishedCb, this ,_1,_2));
        fl_rotate_status = false;
    }
}



void Global_control_system::angle_count()
{
    //Пересчет смещения цели на камере в углы
    a_yaw = std::atan(x_sm/focal_length_x);
    a_pitch = std::atan(y_sm/focal_length_y);
    goal_rotate.alpha_yaw = a_yaw;
    goal_rotate.alpha_pitch = a_pitch;
}
