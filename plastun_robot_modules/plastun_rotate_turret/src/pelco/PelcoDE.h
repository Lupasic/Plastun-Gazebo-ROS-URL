/*
 * PelcoDE.h
 *
 *  Created on: 23.10.2013
 *      Author: user
 */

#ifndef PELCODE_H_
#define PELCODE_H_

#include "PelcoD.h"

namespace pelcode
{

class PelcoDE: public PelcoD
{
public:
	uint8_t* StepLeft(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x41, 0x00, 0x00);
	}
	uint8_t* StepRight(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x43, 0x00, 0x00);
	}
	uint8_t* StepDown(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x45, 0x00, 0x00);
	}
	uint8_t* StepUp(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x47, 0x00, 0x00);
	}
	uint8_t* StepZoomPlus(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x49, 0x00, 0x00);
	}
	uint8_t* StepZoomMinus(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x4b, 0x00, 0x00);
	}
	uint8_t* StepFocusPlus(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x4d, 0x00, 0x00);
	}
	uint8_t* StepFocusMinus(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x4f, 0x00, 0x00);
	}
	uint8_t* PanRequest(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x51, 0x00, 0x00);
	}
	uint8_t* TiltRequest(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x53, 0x00, 0x00);
	}
	uint8_t* MaxPanRequest(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x55, 0x00, 0x00);
	}
	uint8_t* MaxTiltRequest(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x57, 0x00, 0x00);
	}
	uint8_t* ZoomRequest(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x59, 0x00, 0x00);
	}
	uint8_t* FocusRequest(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x5b, 0x00, 0x00);
	}
	uint8_t* MaxZoomRequest(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x5d, 0x00, 0x00);
	}
	uint8_t* MaxFocusRequest(uint16_t deviceAddress)
	{
		return GetMessage(deviceAddress, 0x00, 0x5f, 0x00, 0x00);
	}
	uint8_t* SetPan(uint16_t deviceAddress, uint16_t value)
	{
		return GetMessage(deviceAddress, 0x00, 0x71, (uint8_t)(value>>8), uint8_t(value) );
	}
	uint8_t* SetTilt(uint16_t deviceAddress, uint16_t value)
	{
		return GetMessage(deviceAddress, 0x00, 0x73, (uint8_t)(value>>8), uint8_t(value) );
	}
	uint8_t* SetZoom(uint16_t deviceAddress, uint16_t value)
	{
		return GetMessage(deviceAddress, 0x00, 0x75, (uint8_t)(value>>8), uint8_t(value) );
	}
	uint8_t* SetFocus(uint16_t deviceAddress, uint16_t value)
	{
		return GetMessage(deviceAddress, 0x00, 0x77, (uint8_t)(value>>8), uint8_t(value) );
	}
	uint8_t* SetRegister(uint16_t deviceAddress, uint8_t reg, uint8_t value)
	{
		return GetMessage(deviceAddress, 0x00, 0x9D, reg, value );
	}
	uint8_t* RequestRegister(uint16_t deviceAddress, uint8_t reg)
	{
		return GetMessage(deviceAddress, 0x00, 0x9D, 0, reg );
	}
	uint8_t* LoadPan(uint16_t deviceAddress, uint16_t value)
	{
		return GetMessage(deviceAddress, 0x00, 0x81, (uint8_t)(value>>8), uint8_t(value)  );
	}
	uint8_t* LoadTilt(uint16_t deviceAddress, uint16_t value)
	{
		return GetMessage(deviceAddress, 0x00, 0x83, (uint8_t)(value>>8), uint8_t(value)  );
	}
	uint8_t* SetLoad(uint16_t deviceAddress, uint8_t speed=0x1F)
	{
		return GetMessage(deviceAddress, 0x00, 0x87, 0, speed );
	}
	template<uint8_t cmd>
	bool getValue(uint8_t* msg, uint16_t& val, uint8_t& addr )
	{
		if ( (msg[2] != 0) || (msg[3] != cmd) )
			return false;
		addr = msg[1];
		val = msg[4];
		val = (val <<8);
		val += msg[5];
		return true;
	}
	//reply analyzers returns true if good reply
	bool getPan(uint8_t* msg, uint16_t& val, uint8_t& addr)
	{
		return getValue<0x61>(msg, val, addr);
	}
	bool getTilt(uint8_t* msg, uint16_t& val, uint8_t& addr)
	{
		return getValue<0x63>(msg, val, addr);
	}
	bool getMaxPan(uint8_t* msg, uint16_t& val, uint8_t& addr)
	{
		return getValue<0x65>(msg, val, addr);
	}
	bool getMaxTilt(uint8_t* msg, uint16_t& val, uint8_t& addr)
	{
		return getValue<0x67>(msg, val, addr);
	}
	bool getZoom(uint8_t* msg, uint16_t& val, uint8_t& addr)
	{
		return getValue<0x69>(msg, val, addr);
	}
	bool getFocus(uint8_t* msg, uint16_t& val, uint8_t& addr)
	{
		return getValue<0x6b>(msg, val, addr);
	}
	bool getMaxZoom(uint8_t* msg, uint16_t& val, uint8_t& addr)
	{
		return getValue<0x6d>(msg, val, addr);
	}
	bool getMaxFocus(uint8_t* msg, uint16_t& val, uint8_t& addr)
	{
		return getValue<0x6f>(msg, val, addr);
	}

	bool getCmdComplete(uint8_t* msg,  uint8_t& addr)
	{
		uint16_t val;
		return getValue<0x7c>(msg, val, addr);
	}
	bool getRegister(uint8_t* msg, uint16_t& val, uint8_t& addr)
	{
		return getValue<0xaf>(msg, val, addr);
	}
};

} /* namespace pelcode */
#endif /* PELCODE_H_ */
