#include "move_control.h"

int main(int argc, char** argv)
{
    ros::init(argc,argv,"move_control");
    Move_control *p = new Move_control();
    while(1)
        p->update();

    return 0;
}

