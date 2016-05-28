#include "crc.h"

#define LHA_POLY_16		0xA001u

//##ModelId=41AC837703D7
uint16_t* TCRC16_LHA::tb16 = 0;

//##ModelId=41AC86FE000F
void TCRC16_LHA::Update(const uint8_t* buf, const size_t& size)
{
	crc = ( (crc>>8) ^ (crc <<8) );
    for(unsigned i = 0; i < size; i++)
	{
        crc = (crc >> 8) ^ tb16[ crc & 0xFF ];
        crc ^= buf[i] << 8;
	};
    crc = ( (crc>>8) ^ (crc <<8) );
}



//##ModelId=41AC89AD02ED
unsigned TCRC16_LHA::count = 0;

//##ModelId=41AC8A130251
TCRC16_LHA::TCRC16_LHA()
{
	if( !count ) init();
	count++;
}


//##ModelId=41AC8A130260
TCRC16_LHA::TCRC16_LHA(const TCRC16_LHA& right)
{
	crc = right.get_crc();
	count++;
}

//##ModelId=41AC8A13029F
TCRC16_LHA::~TCRC16_LHA()
{
	count--;
	if(!count) delete []tb16;
	tb16 = 0;
}

//##ModelId=41AC8AF202BE
void TCRC16_LHA::init()
{
	if(!tb16) tb16 = new uint16_t[256];
	unsigned i, j, crc, v;
	for(crc = i = 0; i < 256; tb16[i++] = crc)
		for(crc = i, j = 0; j < 8; j++)
		{
			v = -(crc & 1);
			crc >>= 1;
			crc ^= v & LHA_POLY_16;
		};
}

//##ModelId=41AC8F12006C
TCRC16_LHA::TCRC16_LHA(const uint8_t* buf, const size_t& size)
{
	if( !count ) init();
	count++;
	Update(buf, size);
}
