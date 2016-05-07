#include "image_detect.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_detect");
    Image_detect ic(ros::this_node::getName());
    ros::spin();
    return 0;
}
