#include <ros/ros.h>
#include "geometry_msgs/Pose.h"
#include "control_msgs/JointControllerState.h"
#include "std_msgs/Float64.h"
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include <tf2_msgs/TFMessage.h>
#include <actionlib/server/simple_action_server.h>
//#include "../../../../devel/include/plastun_rotate_turret/angleAction.h"
#include "/home/lupasic/Programs/catkin_ws/devel/include/plastun_rotate_turret/angleAction.h"
//новые инклуды
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
//#include <inifile.h>
//из си
#include <list>
#include "pelco/PelcoDE.h"
#include <iostream>
#include <boost/bind.hpp>
#include "PlaneInterpolation.h"
//#include <dioport.h>

using boost::asio::ip::udp;

class Rotate_turret
{
protected:
    //
    float exc_a ;
    float exc_b ;
    float exc_c ;
    float exc_d ;
    float exc_e ;
    float add_ver_angle;
    float add_hor_angle;
    float robotX ;
    float robotY ;
    float robotFi;
    float pan_scale;
    float tilt_scale;
    int pan_sign;
    int tilt_sign;
    float pan_zero;
    float tilt_zero;
    float hor_max;
    float hor_min;
    float ver_max;
    float ver_min;
    int fire_counter;
    std::string ip;
    int port;
    int dest_port;
    int dev_id;
    int aux_id;

    //Новые переменные
    boost::interprocess::interprocess_mutex udp_mutex;
    boost::interprocess::interprocess_condition udp_cond;
    float hor_angle;
    float ver_angle;
    uint16_t pan;
    uint16_t tilt;
    int16_t  addTilt;
    int16_t	 addPan;
    uint16_t panMax;
    uint16_t tiltMax;
    uint16_t cmdpan;
    uint16_t cmdtilt;
    bool pantilt_cmd ;
    bool pantilt_completed;
    bool completed;
    boost::asio::io_service io_service_;
    udp::socket socket_;
    boost::thread io_service_thread;
    enum { max_length = 1024 };
    uint8_t data_[max_length];
    //
    ros::NodeHandle nh;
    actionlib::SimpleActionServer<plastun_rotate_turret::angleAction> *rt_server;
    std::string action_name;
    //plastun_rotate_turret::angleFeedback feedback;
    plastun_rotate_turret::angleResult result;
    plastun_rotate_turret::angleGoal::ConstPtr goal;
    //Работа с удп
    void connectUDP();
    bool dispatchReply();
    void handleReceiveUDP(const boost::system::error_code& error,
                          size_t bytes_recvd);
    bool waitUDP(int ms);
public:
    Rotate_turret(std::string name);
    ~Rotate_turret();
    void goal_R();
    void preempt_R();
    //Работа с НПУ
    void setNPU(float hor, float ver); //Перерасчет градусы в тики
    void setPanTilt(int32_t p, int32_t t, bool nonblock = false);
    void getPan();
    void getTilt();
    void reset();
};
