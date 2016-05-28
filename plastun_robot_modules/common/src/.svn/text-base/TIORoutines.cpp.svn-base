#include "File.h"

//##ModelId=41F266670203
int TIORoutines::Write(void* data, int nbytes) throw( std::logic_error )
{
	int res; 
	if( (res = write(fd, data, nbytes)) == -1 ) 
		throw std::logic_error( "::Write() exception" );
	return res;
}

//##ModelId=41F266670206
int TIORoutines::Read(void* data, int nbytes) throw( std::logic_error )
{
	int res; 
	if( (res = read(fd, data, nbytes)) == -1 )
		throw std::logic_error( "::Read() exception" ); 
	return res;
}

//##ModelId=41F266670241
int TIORoutines::ReadCond(void *buf, unsigned n, unsigned stimeout, unsigned timeout) throw( std::logic_error )
{
	fd_set rfds;
    struct timeval tv;
    int retval;
    
   /* Ждем, пока на вводе (fd 0) что-нибудь
      появится. */
   FD_ZERO(&rfds);
   FD_SET(fd, &rfds);
   /* Ждем не больше  */
   tv.tv_sec = stimeout;
   tv.tv_usec = timeout;

   retval = select(fd+1, &rfds, NULL, NULL, &tv);
   /* Не полагаемся на значение tv! */

   if (!retval)
      return 0;  
	
	return Read( buf, n);
}

//##ModelId=41F266670251
int TIORoutines::Seek(int offs, int mode) throw( std::logic_error )
{
	int res; 
	if( (res = lseek(fd, offs, mode)) != offs )
		throw std::logic_error( "::Lseek() exception" ); 
	return res;
}

//##ModelId=41F266670270
void TIORoutines::Stat(struct stat* buf) throw( std::logic_error )
{
	if(fstat(fd, buf) == -1)
		throw std::logic_error( "::Stat() exception" ); 
}

//##ModelId=41F266670280
int TIORoutines::Len() throw( std::logic_error )
{
	struct stat buf;
	Stat(&buf);
	return buf.st_size; 
}

//##ModelId=41F26CCA01A5
TIORoutines::TIORoutines() : fd(-1), fname("")
{
}
