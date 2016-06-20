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
        listener->lookupTransform("/turrel_vniz", "/map",
                                  ros::Time(0), transform);

    }
    catch (tf::TransformException &ex)
    {
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
    }
    cur_pos_x = transform.getOrigin().x();
    cur_pos_y = transform.getOrigin().y();



    tf::Vector3 targ = transform(tf::Vector3(goal->target_x,goal->target_y,0)); //трансформим координаты

    std::cout << "Текущее положение относительно map по x: " << cur_pos_x <<" по y: " << cur_pos_y  << std::endl;
    std::cout << "Текущее положение относительно turrel_vniz по х: " << targ.x() <<" по y: " << targ.y()  << std::endl;
    std::cout << "Положение цели относительно map по x: " << goal->target_x <<" по y: " << goal->target_y  << std::endl;
    std::cout << "Положение цели относительно turrel_vniz по x: "  << targ.x() <<" по y: " << targ.y()  << std::endl;

    nh.getParam("/general_targeting/lengh_to_cam_xy",lengh_to_cam_xy); //длина плеча в лаунч файле задается
    nh.getParam("/general_targeting/lengh_to_cam_yz",lengh_to_cam_yz); //длина плеча в лаунч файле задается
    lengh_to_target = std::sqrt(std::pow((targ.getY()),2) + std::pow((targ.getX()),2));

    std::cout << "Расстояние до цели, преобразованные коорд: "  << lengh_to_target <<" от мап: " << std::sqrt(std::pow((goal->target_y - cur_pos_y),2) + std::pow((goal->target_x - cur_pos_x),2))  << std::endl;

    //расчет угла поворота по вертикальной оси (y; x)
    alpha = std::asin(targ.getY()/lengh_to_target);
    std::cout << "Угол альфа: " << alpha << std::endl;
    beta = std::asin(lengh_to_cam_xy/lengh_to_target);
    std::cout << "Угол бета: " << beta << std::endl;
    result.angle_yaw = beta + alpha;
    std::cout << "Полученный угол в градусах: " << result.angle_yaw * 57.3 << std::endl;
    result.angle_pitch = 0;
    gt_server->setSucceeded(result);
}
