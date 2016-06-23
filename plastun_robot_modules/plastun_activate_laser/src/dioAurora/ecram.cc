/*
 * ecram.cc
 *
 *  Created on: 21.09.2013
 *      Author: user
 */

#include <sys/io.h>
#include <stdint.h>
#include <iostream>
#include "ecram.h"

void dscInp(uint16_t addr,ULONG* val)
{
	unsigned short* uval = (unsigned short*)val;
	*uval = inw(addr);
}

void dscOutp(uint16_t addr,ULONG val)
{
	unsigned short* uval = (unsigned short*)(&val);
	outw( *uval, addr);
}

#define Read_io_port inw

unsigned long Process_686C_Command_Write(ULONG m_ECCMD, ULONG m_ECDATA)
{
//--------------------------------------------------------------------------
	Int i, temp;
	ULONG m_OutBuf;
//--------------------------------------------------------------------------
	dscInp(0x6C, &m_OutBuf);
	if ((m_OutBuf & 0x00000003) > 0)
	{
		temp = Read_io_port(0x68);
		return 0xFFFFFFFF;
	}

	dscOutp(0x6C, m_ECCMD);
	for (i = 0; i <= 4000; i++)
	{
		dscInp(0x6C, &m_OutBuf);
		if ((m_OutBuf & 0x00000002) == 0)
			break;
	}
	if (i < 3999)
	{
		dscOutp(0x68, m_ECDATA);
		for (i = 0; i <= 4000; i++)
		{
			dscInp(0x6C, &m_OutBuf);
			if ((m_OutBuf & 0x00000002) == 0)
			{
				return 0x00000000;
			}
		}
	}
	if (i > 3999)
		dscInp(0x68, &m_OutBuf);
	return 0xFFFFFFFF;
}
//---------------------------------------------------------------------------
unsigned long Process_686C_Command_Read(ULONG m_ECCMD)
{
	int i, temp;
	ULONG m_OutBuf, m_InBuf;
	dscInp(0x6C, &m_OutBuf);
	if ((m_OutBuf & 0x00000003) > 0)
	{
		temp = Read_io_port(0x68);
		return 0xFFFFFFFF;
	}
	m_InBuf = m_ECCMD;
	dscOutp(0x6C, m_InBuf);
	for (i = 0; i <= 3500; i++)
	{
		dscInp(0x6C, &m_OutBuf);
		if ((m_OutBuf & 0x00000001) > 0)
		{
			temp = Read_io_port(0x68);
			temp = (temp & 0x000000FF);
			return temp;
			break;
		}
	}
	//if (i > 3499)
	//{
		temp = Read_io_port(0x68);
		return 0xFFFFFFFF;
	//}
}
//----------------------------------------------------------------------------

unsigned long ECU_Read_686C_RAM_BYTE(ULONG ECUMemAddr)
{
	ULONG uDATA1, uDATA2, ECRamAddrH, ECRamAddrL;
	ECRamAddrL = ECUMemAddr % 256;
	ECRamAddrH = ECUMemAddr / 256;
//
	uDATA1 = Process_686C_Command_Write(0x000000A3, ECRamAddrH);
	if (uDATA1 == 0xFFFFFFFF)
	{
		return 0xFFFFFFFF;
	}
//
	uDATA1 = Process_686C_Command_Write(0x000000A2, ECRamAddrL);
	if (uDATA1 == 0xFFFFFFFF)
	{
		return 0xFFFFFFFF;
	}
//
	uDATA1 = Process_686C_Command_Read(0x000000A4);
	if (uDATA1 > 0x000000FF)
	{
		return 0xFFFFFFFF;
	}
	uDATA2 = Process_686C_Command_Read(0x000000A4);
	if (uDATA2 > 0x000000FF)
	{
		return 0xFFFFFFFF;
	}
	if (uDATA1 == uDATA2)
		return uDATA1;
	else
		return 0xFFFFFFFF;
}
//----------------------------------------------------------------------------
unsigned long ECU_Write_686C_RAM_BYTE(ULONG ECUMemAddr, ULONG ECUMemData)
{
	ULONG uDATA,  ECRamAddrH, ECRamAddrL;
	ECRamAddrL = ECUMemAddr % 256;
	ECRamAddrH = ECUMemAddr / 256;
//
	uDATA = Process_686C_Command_Write(0x000000A3, ECRamAddrH);
	if (uDATA == 0xFFFFFFFF)
	{
		return 0xFFFFFFFF;
	}
//
	uDATA = Process_686C_Command_Write(0x000000A2, ECRamAddrL);
	if (uDATA == 0xFFFFFFFF)
	{
		return 0xFFFFFFFF;
	}
//
	uDATA = Process_686C_Command_Write(0x000000A5, ECUMemData);
	if (uDATA == 0xFFFFFFFF)
	{
		return 0xFFFFFFFF;
	}
//
	return 0x00000000;
}
//----------------------------------------------------------------------------

