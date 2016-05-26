#include "general_targeting.h"

int main(int argc, char** argv)
{
    ros::init(argc,argv,"general_targeting");
    General_targeting *p = new General_targeting(ros::this_node::getName());
    ros::spin();
    return 0;
}
