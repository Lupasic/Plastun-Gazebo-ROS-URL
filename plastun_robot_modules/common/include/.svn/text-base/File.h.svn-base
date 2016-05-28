/*************************************************************************
 * File.h created at 09.01.2005 20:40:49
 * ������ ������� ��� ������ � �������
 * This is a part of "QNX Classes" project
 * (c) by Sergei Khandrikov 2003-2005
 *
 ************************************************************************/

#ifndef QNX_CLASSES_FILE_H_INCLUDED
	#define QNX_CLASSES_FILE_H_INCLUDED

#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>

//##ModelId=41F2658202FD
class TIORoutines
{
protected:
	//##ModelId=41F2669900BA
	int fd;

	//##ModelId=41F2669900BB
	std::string fname;

public:
	//��� �����
	//##ModelId=41F2666701E3
	inline const std::string& Name() const { return fname;};

	// 
	//##ModelId=41F2666701E4
	operator int() { return fd;};

	// 
	//##ModelId=41F2666701F3
	operator std::string()	{ return fname;	};

	//���������� �����
	//##ModelId=41F2666701F4
	int Descr() { return fd;};

	//������ � ����
	//##ModelId=41F266670203
	int Write(void* data, int nbytes) throw( std::logic_error );

	//������ �� �����
	//##ModelId=41F266670206
	int Read(void* data, int nbytes) throw( std::logic_error );

	//##ModelId=41F266670241
	int ReadCond(void *buf,unsigned n, unsigned stimeout, unsigned timeout) throw( std::logic_error );

	//��������� ��������
	//##ModelId=41F266670251
	virtual int Seek(
		//��������
		int offs = 0, 
		//����� �������
		int mode = SEEK_SET) throw( std::logic_error );

	//��������� �����, ��. ������� fstat()
	//##ModelId=41F266670270
	void Stat(struct stat* buf) throw( std::logic_error );

	//����� ����� � ������
	//##ModelId=41F266670280
	int Len() throw( std::logic_error );

	//##ModelId=41F266670281
	virtual int Dup() throw( std::logic_error ) = 0;
	
	//##ModelId=41F26CCA01A5
	TIORoutines();

	//##ModelId=4228B1620379
	virtual ~TIORoutines() throw(){};
};


//����� ������ � ������
//##ModelId=41E1751E03A8
class	TFile : public TIORoutines
{	
public :
	
	//�������� �����
	//##ModelId=41E1751F0022
	void		Open(
						//��� �����
						const std::string& path, 
						//����� �������� �����
						int flags) throw( std::logic_error );
						
	//�������� �����
	//##ModelId=41E1751F002D
	void		Open(
						//��� �����
						const std::string& path, 
						//����� �������� �����
						int flags, 
						//��� ��� ��������
						int mode) throw( std::logic_error );
	
	//##ModelId=41F2693101D4
	int Dup() throw( std::logic_error );
						
	//�������� �����
	//##ModelId=41E1751F0031
	void		Close() throw();
	
	//����������� �����������
	//##ModelId=41E1751F0032
	TFile(const TFile& F) throw( std::logic_error );

	//����������� ��-���������
	//##ModelId=41E1751F0034
	TFile();

	//����������� ������� - ��������� ����, ������� Open()
	//##ModelId=41E1751F0035
	TFile(
		//��� �����
		const std::string& path, 
		//����� �������� �����
		int flags) throw( std::logic_error );
		
	//����������� ������� - ��������� ����, ������� Open()
	//##ModelId=41E1751F003F
	TFile(
		//��� �����
		const std::string& path, 
		//����� �������� �����
		int flags, 
		//����� �������� �����
		int mode) throw( std::logic_error );
		
	//����������
	//##ModelId=41E1751F0043
	virtual ~TFile() throw();
};

#endif//#ifndef QNX_CLASSES_FILE_H_INCLUDED
