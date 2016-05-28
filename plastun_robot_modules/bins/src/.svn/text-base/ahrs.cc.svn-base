/*
 * urg.cc
 *
 *  Created on: 11.10.2010
 *      Author: andrey
 */


#include "AHRSManager.h"
#include <sys/mman.h>
#include <inifile.h>
#include <cstdlib>
#include <cstdio>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#define TASK_NAME "AHRSManager"


void term_handler(int i)
{
	printf ("Terminating\n");
	exit(EXIT_SUCCESS);
}


int main(int argc, char* argv[])
{
	struct sigaction sa;
//	sigset_t newset;
//	sigemptyset(&newset);
//	sigaddset(&newset, SIGHUP);
//	sigprocmask(SIG_BLOCK, &newset, 0);
	sa.sa_handler = term_handler;
	sigaction(SIGTERM, &sa, 0);
	sigaction(SIGINT, &sa, 0);

	bool p = false;
	bool v = false;
	std::string cfg;

	ros::init(argc, argv, "bins");
	ros::NodeHandle node;
	ros::Publisher pub = node.advertise<sensor_msgs::Imu>("imu", 100);

        std::string fullName;
        fullName="cfgfile";
        ROS_INFO_STREAM("fullName="<<fullName);
        if ( !node.getParam(fullName, cfg))
        {
          cfg = "mobile.cfg";
        }
        ROS_INFO_STREAM("log name is: " <<cfg);
        std::cout<<"load "<<cfg<<std::endl;
        TINIFile CFG(cfg);
        try
        {
                CFG.Load();
        }
        catch (std::logic_error& E)
        {
                std::cout<<TASK_NAME<<" error load config"<<E.what()<<std::endl;
                exit(0);
        }
	AHRSManager Man(pub);


	if ( ! Man.init(CFG) )
	{
		std::cout<<" can not init ahrs"<<std::endl;
		return 0;
	}

	Man.startManager();

	if (v)
		Man.setVerbose();

	ros::spin();

	Man.stopManager();
	Man.waitfor();
}

