/*
 * COMcommands.h
 *
 *  Created on: 04 авг. 2014 г.
 *      Author: viki
 */

#ifndef COMCOMMANDS_H_
#define COMCOMMANDS_H_

#include <stdint.h>

//message from controller to pc and back
#define MAX_NAVANSDATASIZE  200
#define MAX_NAVREQDATASIZE  20
//if not commented cmd is ignored for testing
//#define TEST_DIRECT_CONNECTION

#define UARTCMD_MARKER      0xFFFF

#ifdef TEST_DIRECT_CONNECTION //
#define UARTCMD_REQ         0x0000  //request
#define UARTCMD_ANS         0x0000  //answer
#define UARTCMD_CONSOLE     0x0000  //answer
#define UARTCMD_ANS_STATE   0x0000
#else
#define UARTCMD_REQ         0x0000  //request
#define UARTCMD_ANS         0x0100  //answer
#define UARTCMD_CONSOLE     0x0200  //answer
#define UARTCMD_ANS_STATE   (UARTCMD_ANS|0x01)
#endif


#define UARTCMD_REQ_STATE            (UARTCMD_REQ|0x01)
#define UARTCMD_REQ_SETVIDEODATETIME (UARTCMD_REQ|0x02)   //время
#define UARTCMD_REQ_SETVIDEOSWITCH   (UARTCMD_REQ|0x03)
#define UARTCMD_REQ_SETVIDEOPIP      (UARTCMD_REQ|0x04)
#define UARTCMD_REQ_SETVIDEOMODE     (UARTCMD_REQ|0x05)
#define UARTCMD_REQ_SETSUBTITLES     (UARTCMD_REQ|0x06)  //установка субтитров
#define UARTCMD_REQ_SETZOOMSTATE     (UARTCMD_REQ|0x07)
#define UARTCMD_REQ_SETIRMODE        (UARTCMD_REQ|0x08) //гамма
#define UARTCMD_REQ_SENDCOMMAND      (UARTCMD_REQ|0x10)
#define UARTCMD_REQ_AUDIOSWITCH      (UARTCMD_REQ|0x09)
#define UARTCMD_REQ_VIDEOONOFF	     (UARTCMD_REQ|0x0C)

#define UARTCMD_CON_CALIBRIGHTMAX    (UARTCMD_CONSOLE|0x03)   //ñîõðàíåíèå ìàêñèìàëüíûõ çíà÷åíèé äëÿ ïðàâîãî äæîéñòèêà
#define UARTCMD_CON_CALIBRIGHTMIN    (UARTCMD_CONSOLE|0x04)   //ñîõðàíåíèå ìèíèìàëüíûõ çíà÷åíèé äëÿ ïðàâîãî äæîéñòèêà
#define UARTCMD_CON_CALIBLEFTMAX     (UARTCMD_CONSOLE|0x05)   //ñîõðàíåíèå ìàêñèìàëüíûõ çíà÷åíèé äëÿ ëåâîãî äæîéñòèêà
#define UARTCMD_CON_CALIBLEFTMIN     (UARTCMD_CONSOLE|0x06)   //ñîõðàíåíèå ìèíèìàëüíûõ çíà÷åíèé äëÿ ëåâîãî äæîéñòèêà
#define UARTCMD_CON_CALIBSAVE        (UARTCMD_CONSOLE|0x07)

#define UARTCMD_CON_ZOOMNOFOCUS      (UARTCMD_CONSOLE|0x01)   //управление джойстиком зумом и фокусом
#define UARTCMD_CON_CALIBRATE        (UARTCMD_CONSOLE|0x02)   //управление калибровкой джорйстиков

#pragma pack(1)
struct DATETIME
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day; //(WWWM MMMM)
	uint8_t month;
	uint8_t year;
	uint8_t cent;
};
//basic structure
struct com_msg_t
{
	uint16_t start;
	uint16_t cmd;
	uint32_t size; // size of drives message + nav message
};
//usual message
struct controller_msg_t : public com_msg_t
{
	union
	{
		uint16_t byte;
		struct
		{
			bool automatic : 1;
			bool iroff:1;
			bool radioOff:1;
			bool subtitles:1;
			bool firsttime:1;
			bool wifioff:1;
			bool eqiupoff:1;
			bool res1:1;
			bool connectionLost:1;
			bool joyCalibrate:1;
		}flags;
	} state;
	int8_t	leftMotor;
	int8_t	rightMotor;
	int8_t	rotatorMotor;
	int8_t	tiltMotor;
	int8_t	focus;
	int8_t	zoom;
	uint8_t frwd_left;
	uint8_t frwd_right;
	uint8_t bcwd_left;
	uint8_t bcwd_right;
};

struct com_data_msg_t :public com_msg_t
{
	DATETIME dt;
};

struct com_byte_msg_t :public com_msg_t
{
	uint8_t byte;
};

//uint8_t buf[]; with nav data
#pragma pack()





#endif /* COMCOMMANDS_H_ */
