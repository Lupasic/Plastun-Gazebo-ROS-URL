#include "image_converter.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_converter");
    ImageConverter ic;
    int a;
    while(1)
    {
        std::cin >> a;
        if(a==0)
        {
            ros::spinOnce();
        }
    }
    return 0;
}
