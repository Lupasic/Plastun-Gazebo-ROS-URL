/*
 * AHRSManager.h
 *
 *  Created on: 11.10.2010
 *      Author: andrey
 */

#ifndef URGMANAGER_H_
#define URGMANAGER_H_

#include <TThread.h>
#include <inifile.h>
#include <File.h>
#include <math.h>
#include <termios.h>
#include <cstring>
#include <ros/ros.h>
#include "ahrs.h"



class AHRSManager: public TThread
{
protected:
	struct termios oldtio, newtio;
	//device
	TFile ahrs;
	ros::Publisher& pub;
	bool verbose;

	uint8_t ahrs_cmd;
	volatile bool m_active;
	bool checkSum(uint16_t* buf, unsigned int size, uint16_t chk);
	uint16_t sum(uint16_t* buf, unsigned int size);
	//reads message returns without header and check sum
	int receiveMessage(void* buf, unsigned int size);
	//sends message with added header and checksum
	bool sendMessage(void* buf, unsigned int size);

	void flush();
	long int tpar;
	long int tdat;
	float dev_roll;
	float dev_pitch;
public:
	void setVerbose(){ verbose = true; }
	void startManager()
	{
		if (m_active) return ;
		m_active = true;
		Run();
	}
	void stopManager()
	{
		m_active = false;
	}
	bool init(TINIFile& CFG);
	virtual void* Main();
	AHRSManager(ros::Publisher& _pub): pub(_pub), verbose(false),m_active(false){}
	virtual ~AHRSManager();
};

#endif /* URGMANAGER_H_ */
