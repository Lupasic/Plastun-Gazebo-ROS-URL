#include "rotate_turret.h"

int main(int argc, char** argv)
    {
    ros::init(argc,argv,"rotate_turret");
    //Rotate_turret *p = new Rotate_turret(1.02,6);
    Rotate_turret p(1.02,5.9);
    while(1)
    {
    sleep(2);
    p.update();
    }
        return 0;
    }
