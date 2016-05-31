#include "rotate_turret.h"
#define COUT(x) std::cout<<"SpbNPUMR:"<<x<<std::endl

using namespace boost::interprocess;

Rotate_turret::Rotate_turret(std::string name):
    dev_id(1),
    aux_id(1),
    pan(0),
    tilt(0),
    pantilt_cmd(false),
    pantilt_completed(true),
    completed(true),
    socket_(io_service_)
{
    //Инициализация переменных
    nh.getParam("/rotate_turret/ip",ip);
    nh.getParam("/rotate_turret/port", port);
    nh.getParam("/rotate_turret/dest_port",dest_port);

    int a;
    nh.getParam("/rotate_turret/panMax",a);
    panMax = a;
    nh.getParam("/rotate_turret/pan_zero",pan_zero);
    nh.getParam("/rotate_turret/tiltMax",a);
    tiltMax = a;
    nh.getParam("/rotate_turret/tilt_zero",tilt_zero);
    nh.getParam("/rotate_turret/pan_sign",pan_sign);
    nh.getParam("/rotate_turret/tilt_sign",tilt_sign);
    pan_scale = 1.0 / (M_PI*2.0) * panMax;
    tilt_scale = pan_scale;

    nh.getParam("/rotate_turret/hor_max",hor_max);
    nh.getParam("/rotate_turret/hor_min",hor_min);
    nh.getParam("/rotate_turret/ver_max",ver_max);
    nh.getParam("/rotate_turret/ver_min",ver_min);
    //
    action_name = name;
    rt_server = new  actionlib::SimpleActionServer<plastun_rotate_turret::angleAction>(nh,name,false);
    //register the goal and feeback callbacks
    rt_server->registerGoalCallback(boost::bind(&Rotate_turret::goal_R,this));
    rt_server->registerPreemptCallback(boost::bind(&Rotate_turret::preempt_R,this));
    // Add your ros communications here.
    nh.getParam("/rotate_turret/rotate_topic", rotate_topic);
    rotate_angle = nh.subscribe(rotate_topic, 1000,&Rotate_turret::rotate_angle_Callback, this);
    rt_server->start();
    //
    connectUDP();
    setNPU(0,0);

}

Rotate_turret::~Rotate_turret()
{
    io_service_.stop();
    COUT("wait io_service");
    io_service_thread.join();
    COUT("udp stopped");

}

void Rotate_turret::goal_R()
{
    goal = rt_server->acceptNewGoal();
    setNPU(goal->alpha_yaw, goal->alpha_pitch);
    result.status = 1;
    //сюда прога попадет, только как выполнит установку
    rt_server->setSucceeded(result);
}

void Rotate_turret::preempt_R()
{
    ROS_INFO("%s: Preempted", action_name.c_str());
    // set the action state to preempted
    rt_server->setPreempted();
}

void Rotate_turret::rotate_angle_Callback(const geometry_msgs::Point &msg)
{
    setNPU(msg.x, msg.y);
    ROS_INFO("Rotate turret complete");
}


void Rotate_turret::connectUDP()
{
    socket_.open(boost::asio::ip::udp::v4());
    boost::asio::ip::udp::endpoint endpoint(
                boost::asio::ip::address::from_string(ip), port);
    boost::system::error_code ec;
    socket_.connect(endpoint, ec);
    if (ec)
    {
        COUT("error connect to udp "<<ip<<":"<<port);
    }
    socket_.async_receive(
                boost::asio::buffer(data_, max_length),
                boost::bind(&Rotate_turret::handleReceiveUDP, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    io_service_thread =
            boost::thread(boost::bind(&boost::asio::io_service::run,
                                      &io_service_) );
    io_service_thread.detach();
}

bool Rotate_turret::dispatchReply()
{
    uint16_t val;
    uint8_t addr;
    pelcode::PelcoDE cam;
    if ( cam.getMaxPan((unsigned char*)data_, val, addr) )
    {
        COUT("max pan "<<val);
        pan = val;
        return true;
    }
    if ( cam.getMaxTilt((unsigned char*)data_, val, addr) )
    {
        COUT("max tilt "<<val);
        tilt = val;
        return true;
    }
    if ( cam.getTilt((unsigned char*)data_, val, addr) )
    {
        COUT("tilt "<<val);
        tilt = val;
        return true;
    }
    if ( cam.getPan((unsigned char*)data_, val, addr) )
    {
        COUT("pan "<<val);
        pan = val;
        return true;
    }
    if ( cam.getCmdComplete((unsigned char*)data_, addr) )
    {
        COUT("udp cmd complete ");
        pantilt_completed = true;
        //		if ( pantilt_cmd )
        //		{
        //			setPanTilt(cmdpan, cmdtilt);
        //		}

        return true;
    }
    return false;
}

void Rotate_turret::handleReceiveUDP(const boost::system::error_code &error, size_t bytes_recvd)
{
    COUT("handle receive UDP");
    if ( ! error && bytes_recvd > 0)
    {
        scoped_lock<interprocess_mutex> lock(udp_mutex);
        dispatchReply();
        udp_cond.notify_all();
        memset(data_, 0, max_length);
    }
    socket_.async_receive(
                boost::asio::buffer(data_, max_length),
                boost::bind(&Rotate_turret::handleReceiveUDP, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

bool Rotate_turret::waitUDP(int ms)
{
    boost::system_time const timeout=
            boost::get_system_time()+ boost::posix_time::milliseconds(ms);
    scoped_lock<interprocess_mutex> lock(udp_mutex);
    //free mutex wile waiting
    return udp_cond.timed_wait(lock, timeout);
}


void Rotate_turret::setNPU(float hor, float ver)
{
    COUT("setNPU "<<hor<<" "<<ver);
    hor_angle = hor;
    ver_angle = ver;
    if ( hor > hor_max) hor = hor_max;
    else 	if ( hor < hor_min ) hor = hor_min;
    if ( ver > ver_max ) ver = ver_max;
    else	if ( ver < ver_min ) ver = ver_min;
    std::cout << pan_scale << "  " << pan_sign << "  " << hor << std::endl;
    int cmd_pan = hor * pan_scale * pan_sign + pan_zero;
    int cmd_tilt = ver * tilt_scale * tilt_sign + tilt_zero;
    std::cout << cmd_pan << "  tilt  " << cmd_tilt << std::endl;
    setPanTilt(cmd_pan, cmd_tilt, true);
}


void Rotate_turret::setPanTilt(int32_t p, int32_t t, bool nonblock)
{
    if (!pantilt_completed)
    {
        COUT("Previous command hasn't completed");
        cmdpan = p;
        cmdtilt = t;
        pantilt_cmd = true;
        return;
    }
    if ( p < 0 )
        p = panMax + p;
    else
        if ( p >= panMax )
            p = p - panMax;
    if ( t < 0 )
        t = 0;
    else
        if ( t> tiltMax)
            t = tiltMax;
    COUT("goto pan: "<<p<<" tilt: "<<t);
    pantilt_completed = false;
    completed = false;
    pantilt_cmd = false;
    pelcode::PelcoDE cam;
    socket_.send(boost::asio::buffer(
                     cam.LoadPan(dev_id, p),	7) );

    socket_.send(boost::asio::buffer(
                     cam.LoadTilt(dev_id, t),7) );

    socket_.send(boost::asio::buffer(
                     cam.SetLoad(dev_id), 7) );
    if ( ! nonblock)
        if ( waitUDP(15000) )
        {
            COUT("received answer");
        }
}

void Rotate_turret::getPan()
{
    pelcode::PelcoDE cam;
    socket_.send(boost::asio::buffer(cam.PanRequest(dev_id),7));

    if ( waitUDP(15000) )
    {
        COUT("received answer");
    }
}

void Rotate_turret::getTilt()
{
    pelcode::PelcoDE cam;
    socket_.send(boost::asio::buffer(cam.TiltRequest(dev_id),7));
    if ( waitUDP(15000) )
    {
        COUT("received answer");
    }
}

void Rotate_turret::reset()
{
    pelcode::PelcoDE cam;
    socket_.send(boost::asio::buffer(cam.ResetCamera(dev_id),7));

    if ( waitUDP(15000) )
    {
        COUT("received answer");
    }
}
