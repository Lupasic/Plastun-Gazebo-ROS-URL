#include "rotate_turret.h"

int main(int argc, char** argv)
{
    ros::init(argc,argv,"rotate_turret");
    Rotate_turret *p = new Rotate_turret(ros::this_node::getName());
    ros::spin();
    return 0;
}
