#include "File.h"

/*************************************************************************
 * TFile.cpp created at 10.12.2004 20:43:48
 * This is a part of "QNX classes" project
 * (c) by Sergei Khandrikov 2004.
 *
 ************************************************************************/


//##ModelId=41F2693101D4
int	TFile::Dup() throw( std::logic_error )
{
	int res = dup(fd);
	if( res == -1 )
		throw std::logic_error( "::Dup() exception" );
	return res;
}

void	TFile::Open(const std::string& path, int flags) throw( std::logic_error )
{
	if(fd != -1)
		throw std::logic_error( "::Open() exception - file is opened already" );

	if((fd = open( (const char*)path.c_str(), flags )) == -1)
		throw std::logic_error( "::Open() exception" );

	fname = path;
}

void	TFile::Open(const std::string& path, int flags, int mode) throw( std::logic_error )
{
	if(fd != -1)
		throw std::logic_error( "::Open() exception - file is opened already" );

	if((fd = open( (const char*)path.c_str(), flags, mode )) == -1)
		throw std::logic_error( "::Open() exception" );

	fname = path;
}

void	TFile::Close() throw()
{
	if(fd != -1) close(fd);
	fd = -1;
}

TFile::TFile(const TFile& F) throw( std::logic_error )
{
	fd = ((TFile)F).Dup();
	fname = F.fname;
}

TFile::TFile()
{ 	
	
}

TFile::TFile(const std::string& path, int flags) throw( std::logic_error )
{ 	
	fd = -1;
	Open(path, flags );
}

TFile::TFile(const std::string& path, int flags, int mode) throw( std::logic_error )
{ 	
	fd = -1;
	Open(path, flags, mode );
}

TFile::~TFile() throw()
{ 
	Close();
}
