#include "image_converter.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_converter");
    ImageConverter ic(ros::this_node::getName());
    ros::spin();
    return 0;
}
