#include "cascad_testing.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "cascad_testing");
    CascadTesting ic;
    ros::spin();
    return 0;
}
