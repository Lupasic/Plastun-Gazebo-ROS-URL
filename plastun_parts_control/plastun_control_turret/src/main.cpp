#include "control_turret.h"

int main(int argc, char** argv)
{
    ros::init(argc,argv,"control_turret");
    Control_turret *p = new Control_turret(1.02,6);
    //Rotate_turret p(1.02,5.9);
    while(1)
        p->update();

    return 0;
}
