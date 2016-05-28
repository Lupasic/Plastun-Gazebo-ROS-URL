#include "crc.h"

#define CCITT_POLY_16		0x1021

//##ModelId=41AC905F03A9
uint16_t* TCRC16_CCITT::tb16 = 0;

//##ModelId=41AC905F03B8
unsigned TCRC16_CCITT::count = 0;

//##ModelId=41AC906E02ED
void TCRC16_CCITT::init()
{
	unsigned short i, j, crc, v;
	if(!tb16) tb16 = new uint16_t[256];
	for(crc = i = 0; i < 256; tb16[i++] = 0xFFFFu & ((crc<<8)^(crc>>8)) )
		for(crc = i<<8, j = 0; j < 8; j++)
		{
			v = -((crc & 0x8000) !=0);
			crc <<= 1;
			crc ^= v & CCITT_POLY_16;
			crc &= 0xFFFFu;
		};
}

//##ModelId=41AC906E02DD
void TCRC16_CCITT::Update(const uint8_t* buf, const size_t& size)
{
	crc = ( (crc>>8) ^ (crc <<8) );
    for(unsigned i = 0; i < size; i++)
	{
        crc ^= buf[i];
        crc = (crc >> 8) ^ tb16[crc&0xFF];
	}
    crc = ( (crc>>8) ^ (crc <<8) );
}

//##ModelId=41AC906E02EE
TCRC16_CCITT::TCRC16_CCITT()
{
	if( !count ) init();
	count++;
}


//##ModelId=41AC906E02FD
TCRC16_CCITT::TCRC16_CCITT(const TCRC16_Z_modem& right)
{
	crc = right.get_crc();
	count++;
}

//##ModelId=41AC906E030C
TCRC16_CCITT::~TCRC16_CCITT()
{
	count--;
	if(!count) delete []tb16;
	tb16 = 0;
}

//##ModelId=41AC906E031C
TCRC16_CCITT::TCRC16_CCITT(const uint8_t* buf, const size_t& size)
{
	if( !count ) init();
	count++;
	Update(buf, size);
}
