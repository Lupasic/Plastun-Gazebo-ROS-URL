#include "activate_laser.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "activate_laser");
    Activate_laser ic(ros::this_node::getName());
    ros::spin();
    return 0;
}
