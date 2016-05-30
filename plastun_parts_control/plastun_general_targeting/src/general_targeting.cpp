#include "general_targeting.h"




General_targeting::General_targeting(std::string name)
{

    action_name = name;
    gt_server = new  actionlib::SimpleActionServer<plastun_general_targeting::access_targetingAction>(nh,name,false);
    //register the goal and feeback callbacks
    gt_server->registerGoalCallback(boost::bind(&General_targeting::goal_R,this));
    gt_server->registerPreemptCallback(boost::bind(&General_targeting::preempt_R,this));
    gt_server->start();
    listener = new tf::TransformListener();
}

void General_targeting::goal_R()
{
    goal = gt_server->acceptNewGoal();
    main_calculation();
}

void General_targeting::preempt_R()
{
    ROS_INFO("%s: Preempted", action_name.c_str());
    // set the action state to preempted
    gt_server->setPreempted();
}

void General_targeting::main_calculation()
{
    float alpha, beta;
    //Считываем координаты с СК карты до основания турели
    tf::StampedTransform transform;
    ros::Rate rate(10.0);
    try{
        listener->lookupTransform("/map", "/turrel_vniz",
                                  ros::Time(0), transform);

    }
    catch (tf::TransformException &ex)
    {
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
    }
    cur_pos_x = transform.getOrigin().x();
    cur_pos_y = transform.getOrigin().y();
    nh.getParam("/general_targeting/lengh_to_cam_xy",lengh_to_cam_xy); //длина плеча в лаунч файле задается
    nh.getParam("/general_targeting/lengh_to_cam_yz",lengh_to_cam_yz); //длина плеча в лаунч файле задается
    lengh_to_target = std::sqrt(std::pow((goal->target_y - cur_pos_y),2) + std::pow((goal->target_x - cur_pos_x),2));
    //1.57 это 90 град
    //расчет угла поворота по вертикальной оси (y; x)
    std::cout << "Текущие координаты робота: " << cur_pos_y << " ; " << cur_pos_x << std::endl;
    std::cout << "Текущие координаты цели: " << goal->target_y << " ; " << goal->target_x << std::endl;
    std::cout << "Расстояние до цели: " << lengh_to_target << std::endl;
    alpha = std::asin(std::abs(goal->target_y - cur_pos_y)/lengh_to_target);
    std::cout << "Угол альфа: " << alpha << std::endl;
    beta = std::acos(lengh_to_cam_xy/lengh_to_target);
    std::cout << "Угол бета: " << beta << std::endl;
    if((goal->target_y - (cur_pos_y + lengh_to_cam_xy)) < 0)
    {
        std::cout << "Y цели меньше Y робота " << (goal->target_y - cur_pos_y) << std::endl;
        result.angle_yaw = -(1.57 - (beta - alpha));
    }
    else
    {
        std::cout << "Y цели больше Y робота " << (goal->target_y - cur_pos_y) << std::endl;
        result.angle_yaw = beta - (1.57 - alpha);

    }
    std::cout << "Полученный угол в радианах: " << result.angle_yaw  << std::endl;
    //Расчет угла поворота по поперечной оси
    //    result.angle_pitch = std::acos(lengh_to_cam_yz/lengh_to_target) - 1.57 + std::asin(abs(goal->target_x - cur_pos_x)/lengh_to_target);
    //    if((goal->target_x - cur_pos_x) < 0)
    //    {
    //        float a = result.angle_pitch;
    //        result.angle_pitch = -a;
    //    }
    result.angle_pitch = 0;
    gt_server->setSucceeded(result);
}
