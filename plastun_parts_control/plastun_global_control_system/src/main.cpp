#include "global_control_system.h"

int main(int argc, char** argv)
{
    ros::init(argc,argv,"global_control_system");
    Global_control_system *p = new Global_control_system();
    ros::spin();
    return 0;
}

