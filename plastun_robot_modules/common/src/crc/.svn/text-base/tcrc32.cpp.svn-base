#include "crc.h"

#define POLY_32			0xEDB88320ul

//##ModelId=41AC929303D7
uint32_t* TCRC32::tb32 = 0;

//##ModelId=41AC92C0031C
unsigned TCRC32::count = 0;

//##ModelId=41AC930D030C
uint32_t TCRC32::get_crc() const
{
    return ( crc ^ 0xFFFFFFFFul );
}


//##ModelId=41AC930D035A
void TCRC32::Clear()
{
	crc = 0xFFFFFFFFul;
}

//##ModelId=41AC930D031C
TCRC32::operator uint32_t()
{
	return get_crc();
}

//##ModelId=41AC93B8032C
void TCRC32::Update(const uint8_t* buf, const size_t& size)
{
	for(unsigned i = 0; i < size; i++)
    {
        crc ^= buf[i];
        crc = (crc >> 8) ^ tb32[ 0xFF & crc ];
    }
}

//##ModelId=41AC936B02CE
int TCRC32::operator==(const TCRC32& right)
{
	return ( crc == right.crc );
}

//##ModelId=41AC936B031C
int TCRC32::operator!=(const TCRC32& right)
{
	return ( crc != right.crc );
}

//##ModelId=41AC9377002E
TCRC32::TCRC32()
{
	Clear();
	if( !count ) init();
	count++;
}


//##ModelId=41AC9377004D
TCRC32::TCRC32(const TCRC32& right)
{
	crc = right.crc;
	count++;
}

//##ModelId=41AC938C03C8
TCRC32::TCRC32(const uint8_t* buf, const size_t& size)
{
	Clear();
	if( !count ) init();
	count++;
	Update(buf, size);
}


//##ModelId=41AC9377008C
TCRC32::~TCRC32()
{
	count--;
	if(!count) delete []tb32;
	tb32 = 0;
}

//##ModelId=41AC9644029F
void TCRC32::init()
{
	unsigned short i, j;
	unsigned long crc, v;
	if(!tb32) tb32 = new uint32_t[256];
	for(crc = i = 0; i < 256; tb32[i++] = crc)
		for(crc = i, j = 0; j < 8; j++)
		{
			v = -(crc & 1);
			crc >>= 1;
			crc ^= v & POLY_32;
		};
}
