/*
 * PelcoDE.h
 *
 *  Created on: 23.10.2013
 *      Author: user
 */

#ifndef PELCOD_H_
#define PELCOD_H_

#include <unistd.h>
#include <stdint.h>
#include <string.h>
namespace pelcode
{

const uint8_t STX = 0xFF;
const uint8_t FocusNear = 0x01;
const uint8_t IrisOpen = 0x02;
const uint8_t IrisClose = 0x04;
const uint8_t CameraOnOff = 0x08;
const uint8_t AutoManualScan = 0x10;
const uint8_t Sense = 0x80;

const uint8_t PanRight = 0x02;
const uint8_t PanLeft = 0x04;
const uint8_t TiltUp = 0x08;
const uint8_t TiltDown = 0x10;
const uint8_t ZoomTele = 0x20;
const uint8_t ZoomWide = 0x40;
const uint8_t FocusFar = 0x80;

const uint8_t PanSpeedMin = 0x00;
const uint8_t PanSpeedMax = 0xFF;

const uint8_t TiltSpeedMin = 0x00;
const uint8_t TiltSpeedMax = 0x3F;
enum Action
{
	Start, Stop
};
enum LensSpeed
{
	Low = 0x00, Medium = 0x01, High = 0x02, Turbo = 0x03
};
enum Switch
{
	On = 0x01, Off = 0x02
};
enum Focus
{
	Near = FocusNear, Far = FocusFar
};
enum Zoom
{
	Wide = ZoomWide, Tele = ZoomTele
};
enum Tilt
{
	Up = TiltUp, Down = TiltDown
};
enum Pan
{
	Left = PanLeft, Right = PanRight
};
enum Scan
{
	ScanAuto, ScanManual
};
enum Iris
{
	Open = IrisOpen, Close = IrisClose
};
enum AuxAction
{
	AuxSet = 0x09, AuxClear = 0x0B
};
enum PatternAction
{
	PatternStart, PatternStop, PatternRun
};
enum SwitchAction
{
	SwitchAuto = 0x00, SwitchOn = 0x01, SwitchOff = 0x02
};
enum PresetAction
{
	PresetSet, PresetClear, PresetGoto
};

// typedef struct {uint8_t Set; uint8_t Clear;}AuxAction;// ={0x09,0x0B} ;
//typedef struct {uint8_t Start; uint8_t Stop; uint8_t Run;}PatternAction;// ={0x00,0x01,0x02} ;
//typedef struct {uint8_t Auto; uint8_t On; uint8_t Off;}SwitchAction;// ={0x00,0x01,0x02} ;

class PelcoD
{
protected:
	uint8_t Address;
	uint8_t CheckSum;
	uint8_t Command1, Command2, Data1, Data2;
	uint8_t m_transmit_pelco_message[7];
	uint8_t* GetMessage(uint8_t address, uint8_t command1, uint8_t command2, uint8_t data1,
			uint8_t data2)
	{
//			if (address<1 || address>256)
//				throw new Exception("Protocol Pelco D support 256 devices only");

		Address = address;
		Data1 = data1;
		Data2 = data2;
		Command1 = command1;
		Command2 = command2;

//			CheckSum = (uint8_t)(STX ^ Address ^ Command1 ^ Command2 ^ Data1 ^ Data2);
		CheckSum = (uint8_t)(
				(Address % 256) + (Command1 % 256) + (Command2 % 256)
						+ (Data1 % 256) + (Data2 % 256));
		m_transmit_pelco_message[0] = STX;
		m_transmit_pelco_message[1] = Address;
		m_transmit_pelco_message[2] = Command1;
		m_transmit_pelco_message[3] = Command2;
		m_transmit_pelco_message[4] = Data1;
		m_transmit_pelco_message[5] = Data2;
		m_transmit_pelco_message[6] = CheckSum;
		return m_transmit_pelco_message;
	}
public:

	PelcoD()
	{
		;
	}
	virtual ~PelcoD()
	{
		;
	}
	uint8_t* Preset(uint16_t deviceAddress, uint8_t preset, PresetAction action)
	{
		uint8_t m_action;
		switch (action)
		{
		case PresetSet:
			m_action = 0x03;
			break;
		case PresetClear:
			m_action = 0x05;
			break;
		case PresetGoto:
			m_action = 0x07;
			break;
		default:
			m_action = 0x03;
			break;
		}
		return GetMessage(deviceAddress, 0x00, m_action, 0x00, preset);
	}

	uint8_t* Flip(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x07, 0x00, 0x21);
	}

	uint8_t* ZeroPanPosition(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x07, 0x00, 0x22);
	}

	uint8_t* SetAuxiliary(uint16_t deviceAddress, uint8_t auxiliaryID, AuxAction action)
	{
		if (auxiliaryID > 0x08)
			auxiliaryID = 0x08;
		return GetMessage(deviceAddress, 0x00, (uint8_t) action, 0x00, auxiliaryID);
	}

	uint8_t* RemoteReset(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x0F, 0x00, 0x00);
	}
	uint8_t* Zone(uint16_t deviceAddress, uint8_t zone, Action action)
	{
//			if(zone<0x01 & zone>0x08)
//				throw new Exception("Zone value should be between 0x01 and 0x08 include");
		uint8_t m_action;
		if (action == Start)
			m_action = 0x11;
		else
			m_action = 0x13;

		return GetMessage(deviceAddress, 0x00, m_action, 0x00, zone);
	}

	uint8_t* WriteToScreen(uint16_t deviceAddress, char ch, uint8_t pos)
	{
		return GetMessage(deviceAddress, 0x00, 0x15, pos, ch);
	}

	//uint8_t* WriteToScreen(uint16_t deviceAddress,string text)
	//{
	//	if(text.Length > 40)
	//		text = text.Remove(40,text.Length-40);
	//	System.Text.Encoding encoding = System.Text.Encoding.ASCII;
	//	uint8_t[] m_BYTEs = new uint8_t[(text.length())*7];
	//	int i = 0;
	//	uint8_t m_scrPosition;
	//	uint8_t m_ASCIIchr;

	//	foreach(char ch in text)
	//	{
	//		m_scrPosition = (uint8_t)(i/7);
	//		m_ASCIIchr = (uint8_t)ch;
	//		Array.Copy(GetMessage(deviceAddress,0x00,0x15,m_scrPosition,m_ASCIIchr),0,m_BYTEs,i,7);
	//		i = i + 7;
	//	}

	//	return m_BYTEs;
	//}

	uint8_t* ClearScreen(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x17, 0x00, 0x00);
	}

	uint8_t* AlarmAcknowledge(uint16_t deviceAddress, uint16_t alarmID)
	{
		//	if(alarmID < 1 & alarmID>8)
		//	throw new Exception("Only 8 alarms allowed for Pelco P implementation");
		return GetMessage(deviceAddress, 0x00, 0x19, 0x00, (uint8_t) alarmID);
	}

	uint8_t* ZoneScan(uint16_t deviceAddress, Action action)
	{
		uint8_t m_action;
		if (action == Start)
			m_action = 0x1B;
		else
			m_action = 0x1D;
		return GetMessage(deviceAddress, 0x00, m_action, 0x00, 0x00);
	}

	uint8_t* Pattern(uint16_t deviceAddress, PatternAction action)
	{
		uint8_t m_action;
		switch (action)
		{
		case PatternStart:
			m_action = 0x1F;
			break;
		case PatternStop:
			m_action = 0x21;
			break;
		case PatternRun:
			m_action = 0x23;
			break;
		default:
			m_action = 0x23;
			break;
		}
		return GetMessage(deviceAddress, 0x00, m_action, 0x00, 0x00);
	}

	uint8_t* SetZoomLensSpeed(uint16_t deviceAddress, LensSpeed speed)
	{
		return GetMessage(deviceAddress, 0x00, 0x25, 0x00, (uint8_t) speed);
	}

	uint8_t* SetFocusLensSpeed(uint16_t deviceAddress, LensSpeed speed)
	{
		return GetMessage(deviceAddress, 0x00, 0x27, 0x00, (uint8_t) speed);
	}

	uint8_t* ResetCamera(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x29, 0x00, 0x00);
	}
	uint8_t* AutoFocus(uint16_t deviceAddress, SwitchAction action)
	{
		return GetMessage(deviceAddress, 0x00, 0x2B, 0x00, (uint8_t) action);
	}
	uint8_t* AutoIris(uint16_t deviceAddress, SwitchAction action)
	{
		return GetMessage(deviceAddress, 0x00, 0x2D, 0x00, (uint8_t) action);
	}
	uint8_t* AGC(uint16_t deviceAddress, SwitchAction action)
	{
		return GetMessage(deviceAddress, 0x00, 0x2F, 0x00, (uint8_t) action);
	}
	uint8_t* BackLightCompensation(uint16_t deviceAddress, Switch action)
	{
		return GetMessage(deviceAddress, 0x00, 0x31, 0x00, (uint8_t) action);
	}
	uint8_t* AutoWhiteBalance(uint16_t deviceAddress, Switch action)
	{
		return GetMessage(deviceAddress, 0x00, 0x33, 0x00, (uint8_t) action);
	}

	uint8_t* EnableDevicePhaseDelayMode(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x35, 0x00, 0x00);
	}
	uint8_t* SetShutterSpeed(uint16_t deviceAddress, uint8_t speed)
	{
		return GetMessage(deviceAddress, 0x00, 0x37, speed, speed); //Not sure about
	}
	uint8_t* AdjustLineLockPhaseDelay(uint16_t deviceAddress)
	{
//			throw new Exception("Did not implemented");
		return GetMessage(deviceAddress, 0x00, 0x39, 0x00, 0x00);
	}
	uint8_t* AdjustWhiteBalanceRB(uint16_t deviceAddress)
	{
//			throw new Exception("Did not implemented");
		return GetMessage(deviceAddress, 0x00, 0x3B, 0x00, 0x00);
	}
	uint8_t* AdjustWhiteBalanceMG(uint16_t deviceAddress)
	{
//			throw new Exception("Did not implemented");
		return GetMessage(deviceAddress, 0x00, 0x3D, 0x00, 0x00);
	}
	uint8_t* AdjustGain(uint16_t deviceAddress)
	{

//			throw new Exception("Did not implemented");
		return GetMessage(deviceAddress, 0x00, 0x3F, 0x00, 0x00);
	}
	uint8_t* AdjustAutoIrisLevel(uint16_t deviceAddress)
	{
//			throw new Exception("Did not implemented");
		return GetMessage(deviceAddress, 0x00, 0x41, 0x00, 0x00);
	}
	uint8_t* AdjustAutoIrisPeakValue(uint16_t deviceAddress)
	{
//			throw new Exception("Did not implemented");
		return GetMessage(deviceAddress, 0x00, 0x43, 0x00, 0x00);
	}
	uint8_t* Query(uint16_t deviceAddress)
	{
//			throw new Exception("Did not implemented");
		return GetMessage(deviceAddress, 0x00, 0x45, 0x00, 0x00);
	}

	uint8_t* CameraSwitch(uint16_t deviceAddress, Switch action)
	{
		uint8_t m_action = CameraOnOff;
		if ((int)action == (int)SwitchOn)
			m_action = CameraOnOff + Sense;
		return GetMessage(deviceAddress, m_action, 0x00, 0x00, 0x00);

	}

	uint8_t* CameraIrisSwitch(uint16_t deviceAddress, Iris action)
	{
		return GetMessage(deviceAddress, (uint8_t) action, 0x00, 0x00, 0x00);
	}

	uint8_t* CameraFocus(uint16_t deviceAddress, Focus action)
	{
		if (action == Near)
			return GetMessage(deviceAddress, (uint8_t) action, 0x00, 0x00, 0x00);
		else
			return GetMessage(deviceAddress, 0x00, (uint8_t) action, 0x00, 0x00);
	}

	uint8_t* CameraZoom(uint16_t deviceAddress, Zoom action)
	{
		return GetMessage(deviceAddress, 0x00, (uint8_t) action, 0x00, 0x20);
	}

	uint8_t* CameraTilt(uint16_t deviceAddress, Tilt action, uint16_t speed)
	{
		if (speed < TiltSpeedMin)
			speed = TiltSpeedMin;
		if (speed < TiltSpeedMax)
			speed = TiltSpeedMax;

		return GetMessage(deviceAddress, 0x00, (uint8_t) action, 0x00,
				(uint8_t) speed);
	}

	uint8_t* CameraPan(uint16_t deviceAddress, Pan action, uint16_t speed)
	{
		if (speed < PanSpeedMin)
			speed = PanSpeedMin;
		if (speed > PanSpeedMax)
			speed = PanSpeedMax;

		return GetMessage(deviceAddress, 0x00, (uint8_t) action, (uint8_t) speed,
				0x00);
	}

	uint8_t* CameraPanTilt(uint16_t deviceAddress, Pan panAction, uint16_t panSpeed,
			Tilt tiltAction, uint16_t tiltSpeed)
	{


		uint8_t* m_tiltMessage = CameraTilt(deviceAddress, tiltAction, tiltSpeed);
		uint8_t* m_panMessage = CameraPan(deviceAddress, panAction, panSpeed);
		/*m_BYTEs[0] = m_tiltMessage[0];
		 m_BYTEs[1] = m_tiltMessage[1];
		 m_BYTEs[2] = m_tiltMessage[2];
		 m_BYTEs[3] = (uint8_t)(m_tiltMessage[3]+m_panMessage[3]);
		 m_BYTEs[4] = (uint8_t)(m_tiltMessage[4]+m_panMessage[4]);
		 m_BYTEs[5] = (uint8_t)(m_tiltMessage[5]+m_panMessage[5]);
		 m_BYTEs[6] = m_tiltMessage[6];
		 m_BYTEs[7] = m_tiltMessage[7];*/
		return GetMessage(deviceAddress, 0x00,
				(uint8_t)(m_tiltMessage[3] + m_panMessage[3]), m_panMessage[4],
				m_tiltMessage[5]);

	}

	uint8_t* CameraStop(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x00, 0x00, 0x00);
	}

	uint8_t* CameraScan(uint16_t deviceAddress, Scan scan)
	{
		uint8_t m_BYTE = AutoManualScan;
		if (scan == ScanAuto)
			m_BYTE = AutoManualScan + Sense;

		return GetMessage(deviceAddress, m_BYTE, 0x00, 0x00, 0x00);

	}

};

} /* namespace pelcode */
#endif /* PELCOD_H_ */
