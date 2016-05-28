#ifndef MAP_H_
#define MAP_H_
#include <unistd.h>
#include "MapBlock.h"
#include <list>

template <class T>

class Map
{
public:
	typedef MapBlock<T>* ptrMapBlock;
	ptrMapBlock * mb;
	//number of blocks	by one side
	int block_num;
	//block_num / 2 - number of crnter block
	int center_num;
	//size of block in meters
	float block_size;
	//number of cells in a block
	int cell_num;
	//size of every cell
	float cell_size;

	T def;

//number of block in mem cash
	int memBankBlockSize;
	//map x into index of cell
	int x_map_cell(float x)
	{			//number of half of cell			    //cell_size
		return center_num * cell_num + floor( x / (cell_size) );
	}

	int x_map_block(float x)
	{
		return floor( x / block_size );
	}

	bool isMapPoint(float x, float y)
	{
		int i = center_num + floor(x / block_size);
		if ( i >= block_num ) return false;
		if ( i < 0 ) return false;

		int j = center_num + floor(y / block_size);
		if ( j >= block_num ) return false;
		if ( j < 0 ) return false;
		return true;
	}

	inline bool isMapPoint(float x, float y, int& i , int& j)
	{
		i = center_num + floor(x / block_size);
		if ( i >= block_num ) return false;
		if ( i < 0 ) return false;

		j = center_num + floor(y / block_size);
		if ( j >= block_num ) return false;
		if ( j < 0 ) return false;
		return true;
	}

	bool isMapCell(int x, int y)
	{
		int i =  floor((float)x / cell_num);
		if ( i >= block_num ) return false;
		if ( i < 0 ) return false;

		int j =  floor((float)y / cell_num);
		if ( j >= block_num ) return false;
		if ( j < 0 ) return false;
		return true;
	}

	inline bool isMapCell(int x, int y, int& i , int& j)
	{
		i = x / cell_num;
		if ( i >= block_num ) return false;
		if ( i < 0 ) return false;

		j = y / cell_num;
		if ( j >= block_num ) return false;
		if ( j < 0 ) return false;
		return true;
	}

	//get set w/o checking
	inline T& operator()( float x, float y )
	{
		int i = center_num + floor(x / block_size);
		int j = center_num + floor(y / block_size);

		int ib = (x - mb[i + j*block_num]->originX)/cell_size;
		int jb = (y - mb[i + j*block_num]->originY)/cell_size;
		return mb[i + j*block_num]->data[ ib  + jb * cell_num ];
	}

	inline T operator()( float x, float y ) const
	{
		int i = center_num + floor(x / block_size);
		int j = center_num + floor(y / block_size);

		int ib = (x - mb[i + j*block_num]->originX)/cell_size;
		int jb = (y - mb[i + j*block_num]->originY)/cell_size;
		return mb[i + j*block_num]->data[ ib + jb * cell_num ];
	}

	bool isBlock(int i, int j)
	{
		if (i < 0) return false;
		if (j < 0) return false;

		if (i >= block_num) return false;
		if (j >= block_num) return false;

		return true;
	}

	inline ptrMapBlock pBlock(int i, int j)
	{
		return mb[i + j*block_num];
	}

	bool blockExist(int i , int j)
	{
		return mb [ i  + j* block_num ] != NULL;
	}

	bool blockExist(float x, float y)
	{
		int i, j;
		if ( !isMapPoint(x,y, i, j) ) return false;
		return blockExist(i,j);
	}

	void blockCreate(int i, int j)
	{
		if ( blockExist(i,j) ) return;
		//block origin (for constructor)
		float ox = ( i - center_num )* block_size;
		float oy = ( j - center_num )* block_size;
							//1 meter 20 cells by side
		mb  [ i  + j * block_num ] = new MapBlock<T>(ox, oy, block_size, cell_num, def, memBankBlockSize);
		assert(mb[i  + j* block_num]);
	}

	MapBlock<T>* blockCreateEx(int i, int j)
	{
		if ( blockExist(i,j) ) return mb  [ i  + j* block_num ];

		float ox = ( i - center_num )* block_size;
		float oy = ( j - center_num )* block_size;
							//1 meter 20 cells by side
		mb  [ i  + j* block_num ] = new MapBlock<T>(ox, oy, block_size, cell_num, def, memBankBlockSize);
		assert(mb[i + j* block_num]);
		return mb  [ i + j * block_num ];
	}

	void set(float x, float y, const T& val)
	{
		int i,j;

		if ( !isMapPoint(x,y, i, j) ) return;
		int block_ind =  i + j* block_num;
		int cell_ind(0);
		if ( mb [ block_ind ] ==  NULL )
		{
			float ox = ( i - center_num )* block_size;
			float oy = ( j - center_num )* block_size;
								//1 meter 20 cells by side
			mb  [ block_ind ] = new MapBlock<T>(ox, oy, block_size, cell_num, def, memBankBlockSize);
			cell_ind = floor((y - oy)/cell_size) * cell_num +  floor((x - ox)/cell_size);
		}
		else cell_ind = floor( (y - mb[block_ind]->originY)/cell_size )* cell_num +
							floor((x - mb[block_ind]->originX)/cell_size);
		mb[block_ind]->data[cell_ind] = val;
	}

	void setGreatest(float x, float y, const T& val)
	{
		int i,j;
		if ( !isMapPoint(x,y, i, j) ) return;
		int block_ind =  i + j* block_num;
		//jy*cell_num + ix
		int cell_ind(0);
		if ( mb [ block_ind ] ==  NULL )
		{
			float ox = ( i - center_num )* block_size;
			float oy = ( j - center_num )* block_size;
								//1 meter 20 cells by side
			mb [ block_ind ] = new MapBlock<T>(ox, oy, block_size, cell_num, def, memBankBlockSize);
			cell_ind = floor( (y - oy)/cell_size )* cell_num +  floor( (x - ox)/cell_size );
		}
		else
		{
			cell_ind = floor( (y - mb[block_ind]->originY)/cell_size ) * cell_num +
							floor( (x - mb[block_ind]->originX)/cell_size );
			if ( mb[block_ind]->data[cell_ind] > val ) return;
		}
		mb[block_ind]->data[cell_ind] = val;
	}

	inline T& get(float x, float y )
	{
		int i, j;
		if ( !isMapPoint(x,y, i, j) ) return def;
		int block_ind = i + j * block_num;
		if ( mb [ block_ind ] ==  NULL ) return def;
		//else
		return (*(mb [ block_ind ]))(x,y);
	}

	inline T* getPtr(float x, float y )
	{
		int i, j;
		if ( !isMapPoint(x,y, i, j) ) return NULL;

		int block_ind = i + j * block_num;
		if ( mb [ block_ind ] ==  NULL ) return NULL;
		//else
		T& res = (*(mb [ block_ind ]))(x,y);
		return &res;
	}

	//get cell value by cell numbers
	inline T& getCell(int x, int y)
	{
		int i = floor((float)x / cell_num);
		int j = floor((float)y / cell_num);
		if (!isBlock(i,j)) return def;
		if (!blockExist(i,j)) return def;
		return mb[i + j * block_num]->data[x%cell_num + y%cell_num  * cell_num];
	}

	//get pointer to cell by indexes
	inline T* getCellPtr(int x, int y)
	{
		int i = floor((float)x / cell_num);
		int j = floor((float)y / cell_num);
		if (!isBlock(i,j)) return NULL;
		if (!blockExist(i,j)) return NULL;
		return mb[i + j * block_num]->data + (x%cell_num + y%cell_num * cell_num);
	}
	//get cell pointer by block and cell indexes
	T* getCellPtr(int blocki, int blockj, int celli, int cellj)
	{
		return mb[blocki + blockj * block_num]->data + celli + cellj*cell_num;
	}
	//set by cell  numbers
	inline void setCell(int x, int y, const T& val)
	{
		int i = floor( (float)x / cell_num );
		int j = floor( (float)y / cell_num );
		if (!isBlock(i,j)) return;
		if (!blockExist(i,j)) blockCreate(i,j);
		mb[i + j * block_num]->data[x%cell_num + y%cell_num *cell_num] = val;
	}

	void setGreatestCell(int x, int y, const T&  val)
	{
		int i = floor( (float)x / cell_num );
		int j = floor( (float)y / cell_num );
		if (!isBlock(i,j)) return;
		int block_ind(i + j * block_num);
		int cell_ind(x % cell_num + y % cell_num * cell_num);
		if ( !mb[block_ind] )
		{
			//block origin (for constructor)
			float ox = ( i - center_num )* block_size;
			float oy = ( j - center_num )* block_size;
								//1 meter 20 cells by side
			mb  [ block_ind ] = new MapBlock<T>(ox, oy, block_size, cell_num, def, memBankBlockSize);
		}
		else
			if (mb[ block_ind ]->data[cell_ind] > val) return;
		mb[ block_ind ]->data[cell_ind] = val;
	}

	//call Function for each cell in line
	template< class Function >
	void forEachInLine(float x1, float y1, float x2, float y2, Function func )
	{
		if( x1 > x2 )
		{
			std::swap( x1, x2 );
			std::swap( y1, y2 );
		}

		int idY = (y2>=y1)?1:-1;

		float eps = cell_size / 10;

		if (fabs(y1 - y2) < eps ) y2 += idY*eps;
		if (x2 - x1 < eps) x1-= eps;
		float k = (x2 - x1 )/(y2 - y1);


		int i1 = center_num + floor((x1 ) / block_size);
		int j1 = center_num + floor(y1 / block_size);

		int i2 = center_num + floor((x2 ) / block_size);
		int j2 = center_num + floor(y2 / block_size);

		//arrays of points of crossing given line with x = Xi
		std::list <float> crossX;

		//arrays of points of crossing given line with y = Yj
		std::list <float> crossY;

		for(int i = i1; i < i2; i++)
		{
			crossX.push_back(y1 + ((i - center_num + 1)*block_size - x1)/k);
		}
		crossX.push_back(y2);
		if (idY > 0)
		{
			for( int j = j1; j < j2; j++)
			{
				crossY.push_back(x1 + ((j - center_num + 1)*block_size - y1)*k);
			}
		}
		else
		{
			for( int j = j1; j > j2; j--)
			{
				crossY.push_back(x1 + ((j - center_num )*block_size - y1)*k);
			}
		}
		crossY.push_back(x2);
		int i = i1; int j = j1;
		float X = (i - center_num + 1) * block_size;
		float x,y;
		while( i < i2 || idY*j < idY*j2 )
		{
			if ( X < crossY.front() || crossY.size()==1 )
			{ //move right
				y = crossX.front();
				crossX.pop_front();
				x = X;
				if (isBlock(i1,j1))
				{
					if(!blockExist(i1,j1))
						blockCreate(i1,j1);
					mb[i1 + j1 * block_num]->forEachInLine(x1, y1, x - eps , y , func);
				}

				i = i + 1;
				x1 = x + eps;
				y1 = y;
				i1 = i;
				X += block_size;
				continue;
			}
			if ( X > crossY.front() || crossX.size() == 1)
			{//move up or down
				if (idY > 0) y = (j - center_num + 1)*block_size;
				else y = (j - center_num)*block_size;
				x = crossY.front();
				crossY.pop_front();
				if (isBlock(i1,j1))
				{
					if(!blockExist(i1,j1))
						blockCreate(i1,j1);

					mb[i1 + j1 * block_num]->forEachInLine(x1, y1, x , y - idY * eps, func);
				}
				j = j + idY;
				y1 = y + idY*eps;
				x1 = x ;
				j1 = j;
				continue;
			}
			else //unbelievable - move diagonal
			{
				y = crossX.front();
				x = crossY.front();
				crossX.pop_front();
				crossY.pop_front();
				if (isBlock(i1,j1))
				{
					if(!blockExist(i1,j1))
						blockCreate(i1,j1);
					mb[i1 + j1 * block_num]->forEachInLine(x1,y1, x - eps  , y - idY * eps, func);
				}
				i = i + 1;
				j = j + idY;
				y1 = y + idY*eps;
				x1 = x + eps;
				j1 = j;
				i1 = i;
				X += block_size;
			}

		}

		if (isBlock(i1,j1))
		{
			if(!blockExist(i1,j1)) blockCreate(i1,j1);
			mb[i1 + j1 * block_num]->forEachInLine(x1, y1, x2, y2, func);
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
	/*	if( x1 > x2 )
		{
			std::swap( x1, x2 );
			std::swap( y1, y2 );
		}

		int idY = (y2>=y1)?1:-1;

		float eps = cell_size / 10;

		if (fabs(y1 - y2) < eps ) y2 += idY*eps;
		if (x2 - x1 < eps) x1-= eps;
		float k = (x2 - x1 )/(y2-y1);


		int i1 = center_num + floor((x1 ) / block_size);
		int j1 = center_num + floor(y1 / block_size);

		int i2 = center_num + floor((x2 ) / block_size);
		int j2 = center_num + floor(y2 / block_size);

		//arrays of points of crossing given line with x = Xi
		std::list <float> crossX;

		//arrays of points of crossing given line with y = Yj
		std::list <float> crossY;

		for(int i = i1; i < i2; i++)
		{
			crossX.push_back(y1 + ((i - center_num + 1)*block_size - x1)/k);
		}
		crossX.push_back(y2);
		if (idY > 0)
		{
			for( int j = j1; j < j2; j++)
			{
				crossY.push_back(x1 + ((j - center_num + 1)*block_size - y1)*k);
			}
		}
		else
		{
			for( int j = j1; j > j2; j--)
			{
				crossY.push_back(x1 + ((j - center_num )*block_size - y1)*k);
			}
		}
		crossY.push_back(x2);
		int i = i1; int j = j1;
		float X = (i - center_num + 1) * block_size;
		float x,y;
		while( i < i2 || idY*j < idY*j2 )
		{
			if ( X < crossY.front() || crossY.size()==1 )
			{ //move right
				y = crossX.front();
				crossX.pop_front();
				x = X;
				if (isBlock(i1,j1))
				{
					if(!blockExist(i1,j1))
						blockCreate(i1,j1);
					mb[i1 + j1 * block_num]->drawLine(x1,y1, x - eps , y , col);
				}

				i = i + 1;
				x1 = x + eps;
				y1 = y;
				i1 = i;
				X += block_size;
				continue;
			}
			if ( X > crossY.front() || crossX.size() == 1)
			{//move up or down
				if (idY > 0) y = (j - center_num + 1)*block_size;
				else y = (j - center_num)*block_size;
				x = crossY.front();
				crossY.pop_front();
				if (isBlock(i1,j1))
				{
					if(!blockExist(i1,j1))
						blockCreate(i1,j1);

					mb[i1 + j1 * block_num]->drawLine(x1,y1, x , y - idY * eps, col);
				}
				j = j + idY;
				y1 = y + idY*eps;
				x1 = x ;
				j1 = j;
				continue;
			}
			else //unbelievable - move diagonal
			{
				y = crossX.front();
				x = crossY.front();
				crossX.pop_front();
				crossY.pop_front();
				if (isBlock(i1,j1))
				{
					if(!blockExist(i1,j1))
						blockCreate(i1,j1);
					mb[i1 + j1 * block_num]->drawLine(x1,y1, x - eps  , y - idY * eps, col);
				}
				i = i + 1;
				j = j + idY;
				y1 = y + idY*eps;
				x1 = x + eps;
				j1 = j;
				i1 = i;
				X += block_size;
			}

		}

		if (isBlock(i1,j1))
		{
			if(!blockExist(i1,j1)) blockCreate(i1,j1);
			mb[i1 + j1 * block_num]->drawLine(x1, y1, x2, y2, col);
		}
*/
	}

	void Clear()
	{
		for( int i = 0; i< block_num*block_num; i++)
		{
			if ( mb[i] )
			{
				delete mb[i];
				mb[i] = NULL;
			}
		}
	}
	//number of blocks by side
	//size of every block
	//number of cells in block
	Map(int _num, float _block_size, int _cell_num, const T _def , int memBlock=0)
	: block_num(_num),center_num(block_num/2), block_size(_block_size), cell_num(_cell_num),
		cell_size(_block_size/cell_num), def(_def), memBankBlockSize((memBlock>0)?memBlock:block_num)
	{
		mb = new ptrMapBlock[block_num *block_num];

//		for( int i = 0; i< block_num * block_num; i++) mb[i] = NULL;
		memset(mb,0, block_num*block_num*sizeof(ptrMapBlock));

	}
	Map(const Map<T> &old):block_num(old.block_num),center_num(old.center_num), block_size(old.block_size), cell_num(old.cell_num),
	cell_size(old.cell_size), def(old.def)
	{
		mb = new ptrMapBlock[block_num *block_num];

		for( int i = 0; i< block_num * block_num; i++)
			if (old.mb[i] == NULL) mb[i] = NULL;
			else
			{
				mb[i] = new MapBlock<T> (*(old.mb[i]));
			}

	}

	virtual ~Map()
	{
		for( int i = 0; i< block_num*block_num; i++)
		{
			if ( mb[i] )
			{
				delete mb[i];
				mb[i] = NULL;
			}
		}
		delete mb;
	}
};

#endif /*MAP_H_*/
