#include "crc.h"

#define CCITT_POLY_16		0x1021

//##ModelId=41AC879703E7
uint16_t* TCRC16_Z_modem::tb16 = 0;

//##ModelId=41AC8D690280
unsigned TCRC16_Z_modem::count = 0;

//##ModelId=41AC87D00157
void TCRC16_Z_modem::Update(const uint8_t* buf, const size_t& size)
{
	crc = ( (crc>>8) ^ (crc <<8) );
    for(unsigned i = 0; i < size; i++)
	{
        crc = (crc >> 8) ^ tb16[ crc & 0xFF ];
        crc ^= buf[i] << 8;
	};
    crc = ( (crc>>8) ^ (crc <<8) );
}

//##ModelId=41AC8DAB01A5
void TCRC16_Z_modem::init()
{
	unsigned short i, j, crc, v;
	tb16 = new uint16_t[256];
	for(crc = i = 0; i < 256; tb16[i++] = 0xFFFFu & ((crc<<8)^(crc>>8)) )
		for(crc = i<<8, j = 0; j < 8; j++)
		{
			v = -((crc & 0x8000) !=0);
			crc <<= 1;
			crc ^= v & CCITT_POLY_16;
			crc &= 0xFFFFu;
		};
}

//##ModelId=41AC8DB90260
TCRC16_Z_modem::TCRC16_Z_modem()
{
	if( !count ) init();
	count++;
}


//##ModelId=41AC8DB90270
TCRC16_Z_modem::TCRC16_Z_modem(const TCRC16_Z_modem& right)
{
	crc = right.get_crc();
	count++;
}

//##ModelId=41AC8DB902BE
TCRC16_Z_modem::~TCRC16_Z_modem()
{
	count--;
	if(!count) delete []tb16;
	tb16 = 0;
}

//##ModelId=41AC8F870222
TCRC16_Z_modem::TCRC16_Z_modem(const uint8_t* buf, const size_t& size)
{
	if( !count ) init();
	count++;
	Update(buf, size);
}
