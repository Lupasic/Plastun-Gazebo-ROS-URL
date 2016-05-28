#ifndef SERIALIZEHELPER_H_
#define SERIALIZEHELPER_H_

#include <string>
#include <vector>
#include "Archive.h"

template <class T> void Serialize( Archive& ar, std::vector< T >& vData )
{
	int nSize = vData.size();
	ar.push( &nSize, sizeof( nSize ) );

	if( nSize > 0 )
		ar.push( &vData[ 0 ], nSize );
}

template< class T> bool DeSerialize( Archive& ar, std::vector< T >& vData )
{
	int nSize;
	if( !ar.pop( &nSize, sizeof( nSize ) ) )
		return false;

	if( nSize > 0 )
	{
		vData.resize( nSize );
		if( !ar.pop( &vData[ 0 ], nSize ) )
		{
			vData.clear();
			return false;
		}
	}

	return true;
}

#define ARPUSH(x) ar.push(&x, sizeof(x));
#define ARPOP(x) if (! ar.pop(&x, sizeof(x) ) ) return false;

#endif /*SERIALIZEHELPER_H_*/
