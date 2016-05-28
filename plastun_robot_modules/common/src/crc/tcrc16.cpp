#include "crc.h"

//##ModelId=41AC83E300E9
uint16_t TCRC16::get_crc() const
{
    return crc;
}


//##ModelId=41AC83FF0280
TCRC16::operator uint16_t()
{
	return get_crc();
}

//##ModelId=41AC884600F9
TCRC16::TCRC16()
{
	Clear();
}


//##ModelId=41AC88460118
TCRC16::~TCRC16()
{
	
}

//##ModelId=41AC88460138
int TCRC16::operator==(const TCRC16& right)
{
	return ( crc == right.crc );
}

//##ModelId=41AC88460176
int TCRC16::operator!=(const TCRC16& right)
{
	return ( crc != right.crc );
}

//##ModelId=41AC8BBF0280
void TCRC16::Clear()
{
	crc = 0;
}
