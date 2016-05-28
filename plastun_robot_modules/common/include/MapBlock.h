#ifndef MAPBLOCK_H_
#define MAPBLOCK_H_
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include "MemoryBank.h"
template <class T >

class MapBlock
{
protected:
public:
	//origin of block - coordinates of left low angle
	float originX;
	float originY;
	//size of square block meters
	float block_size;
	//number of cells in block
	int cell_num;
	//size of each cell in block
	float cell_size;

	//pointer to array of data;
	T * data;
	//default value
	T def;

	inline T& operator()( float x, float y )
	{
		int i = (x - originX)/cell_size;
		int j = (y - originY)/cell_size;
		return data[ j * cell_num + i ];
	}

	inline T operator()( float x, float y ) const
	{
		int i = (x - originX)/cell_size;
		int j = (y - originY)/cell_size;
		return data[ j * cell_num + i ];
	}

	inline void put(int x, int y, const T& val)
	{
		if ( x < 0) assert(0);
		if ( x >=cell_num ) assert (0);
		if ( y < 0) assert(0);
		if ( y >=cell_num ) assert (0);

		data[y*cell_num + x] = val;
	}

	inline void set(float x, float y, const T& val)
	{
		if ( x < originX ) return;
		if ( x >= originX + block_size) return;
		int i = (x - originX)/cell_size;

		if ( y < originY ) return;
		if ( y >= originY + block_size ) return;
		int j = (y - originY)/cell_size;

	//	if ( height > data[ i*cell_num + j ] )
			data[ j*cell_num + i ] = val;
	}
	inline T& get(float x, float y)
	{

		if ( x < originX ) return def;
		if ( x >= originX + block_size) return def;
		int i = ( (x - originX)/cell_size );

		if ( y < originY ) return def;
		if ( y >= originY + block_size ) return def;
		int j = ( (y - originY)/cell_size );

		return data[ j*cell_num + i ];
	}
	//call Function for each cell in line
	template< class Function >
	void forEachInLine(float x1, float y1, float x2, float y2, Function func )
	{
		int ix1 = (x1-originX)*cell_num/block_size;
		int iy1 = (y1-originY)*cell_num/block_size;

		int ix2 = (x2-originX)*cell_num/block_size;
		int iy2 = (y2-originY)*cell_num/block_size;

		if( ix1 > ix2 )
		{
			std::swap( ix1, ix2 );
			std::swap( iy1, iy2 );
		}
		int x = 0;
		int y = 0;
		int Px = ix2 - ix1;
		int Py = iy2 - iy1;

		bool bPyMoreThanZero = Py > 0;
		if( !bPyMoreThanZero )
			Py = -Py;

		bool bPxMoreThanPy = Px > Py;

		int count = Px  ;
		if( !bPxMoreThanPy )
		{
			count = Py  ;
			std::swap( x, y );
			std::swap( Px, Py );
		}

		int E = 2 * Py - Px;

		func( data[ iy1*cell_num + ix1 ] );


		for( int i = 0; i < count; i++ )
		{
			if (E >= 0)
			{
				x++;
				y++;

				E -= 2 * Px;
			}
			else
				x++;

			E = E + 2 * Py;

			if( bPxMoreThanPy )
				if( bPyMoreThanZero )
					func ( data[ (ix1 + x) + (iy1 + y)*cell_num ] );
				else
					func ( data[ (ix1 + x) + (iy1 - y)*cell_num ] );
			else
				if( bPyMoreThanZero )
					func ( data[ (ix1 + y) + (iy1 + x)*cell_num ] );
				else
					func ( data[ (ix1 + y) + (iy1 - x)*cell_num ] );
		}

	}

	struct assign_class
	{
		const T& col;
		void operator () (T& cell) {cell = col; }
		assign_class(const T& _col) : col(_col){}
	} ;

	void drawLine(float x1, float y1, float x2, float y2, const T& col)
	{
		assign_class assign(col);

		forEachInLine(x1, y1, x2, y2, assign );
/*		int ix1 = (x1-originX)*cell_num/block_size;
		int iy1 = (y1-originY)*cell_num/block_size;

		int ix2 = (x2-originX)*cell_num/block_size;
		int iy2 = (y2-originY)*cell_num/block_size;

		if( ix1 > ix2 )
		{
			std::swap( ix1, ix2 );
			std::swap( iy1, iy2 );
		}
		int x = 0;
		int y = 0;
		int Px = ix2 - ix1;
		int Py = iy2 - iy1;

		bool bPyMoreThanZero = Py > 0;
		if( !bPyMoreThanZero )
			Py = -Py;

		bool bPxMoreThanPy = Px > Py;

		int count = Px  ;
		if( !bPxMoreThanPy )
		{
			count = Py  ;
			std::swap( x, y );
			std::swap( Px, Py );
		}

		int E = 2 * Py - Px;

		data[ iy1*cell_num + ix1 ] = col;


		for( int i = 0; i < count; i++ )
		{
			if (E >= 0)
			{
				x++;
				y++;

				E -= 2 * Px;
			}
			else
				x++;

			E = E + 2 * Py;

			if( bPxMoreThanPy )
				if( bPyMoreThanZero )
					data[ (ix1 + x) + (iy1 + y)*cell_num ] = col;
				else
					data[ (ix1 + x) + (iy1 - y)*cell_num ] = col;
			else
				if( bPyMoreThanZero )
					data[ (ix1 + y) + (iy1 + x)*cell_num ] = col;
				else
					data[ (ix1 + y) + (iy1 - x)*cell_num ] = col;
		}*/

	}

	MapBlock( float x, float y, float _block_size, int num, const T& _def, int memBankSize = 100)
	    : originX(x), originY(y), block_size(_block_size), cell_num(num), cell_size(_block_size/cell_num), def(_def)
	{
		//create memory bank

		MemoryBank<T> *memBank = MemoryBank<T>::getBank(memBankSize, sizeof(T)*cell_num*cell_num );

		data = reinterpret_cast<T*> (memBank->newFromMemBank());

		for (int i = 0;  i < cell_num * cell_num; i++  ) data[i] = def;
	}

	MapBlock(const MapBlock<T> & old)
		: originX(old.originX), originY(old.originY), block_size(old.block_size),
		cell_num(old.cell_num), cell_size(old.cell_size), def(old.def)
	{

		data = reinterpret_cast<T*> (MemoryBank<T>::getBank()->newFromMemBank());
		memcpy(data, old.data, cell_num*cell_num*sizeof(T));
	}

	virtual ~MapBlock()
	{
		MemoryBank<T>::getBank()->deleteToMemBank(data);
	}
};

#endif /*MAPBLOCK_H_*/
