#include "Archive.h"
#include <algorithm>

void Archive::clear()
{
	dataContainer.clear();
}

//##ModelId=4485EB710167
void Archive::push(const void* lpData, unsigned int size)
{
	if( !lpData || size == 0 )
		return;

	char* lpCharData = ( char* )lpData;
	dataContainer.insert( dataContainer.end(), lpCharData, lpCharData + size );
}

//##ModelId=4485EB840251
bool Archive::pop(void* lpData, unsigned int size)
{
	if( dataContainer.size() < size )
		return false;

	memcpy( lpData, getData(), size );
	dataContainer.erase( dataContainer.begin(), dataContainer.begin() + size );

	return true;
}

void Archive::erase( unsigned int size )
{
	size = std::min( size, (unsigned int)dataContainer.size() );

	if( size == 0 )
		return;

	dataContainer.erase( dataContainer.begin(), dataContainer.begin() + size );
}

//##ModelId=44874DE503D8
char* Archive::getData()
{
	return &dataContainer[ 0 ];
}

int Archive::getDataSize()
{
	return ( int )dataContainer.size();
}
