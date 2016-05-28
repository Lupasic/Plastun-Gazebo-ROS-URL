/*******************************************************************************
 * INI file module created by Anddy Minin & Sergei Khandrikov
 * Based on ported from QNX 4.25 INI file module by Anddy Minin
 *
 * (c) by Anddy Minin 2003 - 2004
 * (c) by Khandrikov Sergei 2004-2005
 *
 ******************************************************************************/

#ifndef _INIFILE_HPP_INCLUDED
	#define _INIFILE_HPP_INCLUDED

#include <map>
#include <string>
#include <stdexcept>
#include <ctime>
//#include "base.h"
//#include "exceptions.h"

//##Documentation
//## ����������� ������� INI �����
//##ModelId=41C840A602F0
class	TINIItem
{
protected:
	//##Documentation
	//## ��� ��������
	std::string			name;
public:
	//##Documentation
	//## ������ � ����� ��������
	//##ModelId=41C840A700BC
	std::string&			Name(){ return name;};

	//##Documentation
	//## �����������
	//##ModelId=41C840A700CA
	TINIItem(
		//##Documentation
		//## ���, ������������� ��������
		const std::string& aName){ name = aName;};

	//##Documentation
	//## ����������� �����������
	//##ModelId=41C840A700CC
	TINIItem(const TINIItem& src){ name = src.name;};

	//##Documentation
	//## ����������
	//##ModelId=41C840A700CE
	virtual ~TINIItem(){};
};

//##Documentation
//## ����� - ���� ������
//##ModelId=41C840A602FD
class	TINIValue : public TINIItem
{
protected:
	//##Documentation
	//## �������� ����� � ���������� ����
	std::string		value;
public:
	//##Documentation
	//## ���������� �������� ����� ������������� �����
	//##ModelId=41C840A700FB
	void		IntValue(
		//##Documentation
		//## ��������������� ��������
		int val);

	//##Documentation
	//## ���������������� �������� ����� ��� ������������� ���, ����������
	//## �������� ����� � ���������������� ����.
	//##ModelId=41C840A700FD
	int		IntValue();

	//##Documentation
	//## ���������� �������� ����� ������������� ����� � 16-�������
	//## �������������.
	//##ModelId=41C840A700FE
	void		HexValue(
		//##Documentation
		//## ��������������� ��������
		int val);

	//##Documentation
	//## ���������������� �������� ����� ��� ������������� ��� ���������� �
	//## ����������������� ����, ���������� �������� ����� � ����������������
	//## ����.
	//##ModelId=41C840A70108
	int		HexValue();

	//##Documentation
	//## ���������� �������� ����� ��������� �����
	//##ModelId=41C840A70109
	void		StrValue(
		//##Documentation
		//## ��������������� ��������
		const std::string& val);

	//##Documentation
	//## ���������������� �������� ����� ��� ��������� ���, ���������� ��������
	//## ����� � ���������������� ����.
	//##ModelId=41C840A7010B
	std::string&		StrValue();

	//##Documentation
	//## ���������� �������� ����� ������������ �����
	//##ModelId=41C840A7010C
	void		FloatValue(
		//##Documentation
		//## ��������������� ��������
		float val);

	//##Documentation
	//## ���������������� �������� ����� ��� ������������ ���, ����������
	//## �������� ����� � ���������������� ����.
	//##ModelId=41C840A7010E
	float		FloatValue();

	//##Documentation
	//## ������ � �������� ����� � ���������� ����
	//##ModelId=41C840A7010F
	std::string&		Value(){return value;};

	//##Documentation
	//## ����������� ������� "��-���������"
	//##ModelId=41C840A70118
	TINIValue() : TINIItem("noname"), value("") {};

	//##Documentation
	//## ����������� �������
	//##ModelId=41C840A70119
	TINIValue(
		//##Documentation
		//## ��� �����
		const std::string aName,
		//##Documentation
		//## �������� ����� � ���������� ����
	    const std::string aValue) : TINIItem(aName), value(aValue) {};

	//##Documentation
	//## ����������� �����������
	//##ModelId=41C840A7011C
	TINIValue(const TINIValue& src) : TINIItem(src.name) { value = src.value;};

	//##Documentation
	//## ���������� �������
	//##ModelId=41C840A7011E
	virtual ~TINIValue(){};
};

//##Documentation
//## ��� - ������� ������
//##ModelId=41C840A6030C
typedef std::map<std::string,TINIValue> items_t;

//##Documentation
//## ����� - ������ INI �����
//##ModelId=41C840A602FC
class	TINISection : public TINIItem
{
public:
	//##Documentation
	//## ������ ������ ������.
	//##ModelId=41C840A700DB
	items_t items;

	//##Documentation
	//## �����������
	//##ModelId=41C840A700DF
	TINISection(
		//##Documentation
		//## ��� ����������� ������
		const std::string& aName) : TINIItem(aName) {};

	//##Documentation
	//## ����������� "��-���������"
	//##ModelId=41C840A700E1
	TINISection() : TINIItem("noname") {};

	//##Documentation
	//## ����������� �����������
	//##ModelId=41C840A700E9
	TINISection(const TINISection& src) : TINIItem(src.name) { items = src.items;};

	//##Documentation
	//## ���������� �������
	//##ModelId=41C840A700EB
	virtual ~TINISection(){};
};

//##Documentation
//## ��� - ������� ������
//##ModelId=41C840A6030E
typedef std::map<std::string,TINISection> sects_t;

//##Documentation
//## ����� - ���� ������������ ���� windows ini-file. ���������� � �����
//## ������������� � ������. ������ ���� ��������� � �����-���� ������. ���
//## ������ ������ ��������� �� ������� ���� 1 ������. ��� ����� ������
//## ��������� �� ������� ���� ���� ������.
//##
//## ������ �����:
//##
//## ;����������
//## [%���_������%];����������
//## %���_�����%=%��������%
//##
//## ��������!!! ��� ���������� ����� ���������� ����� �������!
//##ModelId=41C840A602EF
class	TINIFile
{
protected:
	//##Documentation
	//## ������� ��� �����
	std::string		fname;

	//##Documentation
	//## ������� �� ������ ���������� � ���������� �������/���������
	//##ModelId=41C840A70025
	std::string&		TrimStr(
		//##Documentation
		//## �������������� ������
		std::string& str);

	//##Documentation
	//## �������� ������ �� �������������� � ��������� "������" �� ��������
	//## ���������� ���������� ������ � ������ � ��������� ������� ������
	//##ModelId=41C840A7002E
	bool		IsSection(
		//##Documentation
		//## ����������� ������
		const std::string& str);

	//##Documentation
	//## ��������� ������, ���������������� ���������� ���� � ��� ��������.
	//## ���� ������ �� �������� ������ � ����������, �� ������������ ������
	//## ����.
	//##ModelId=41C840A70030
	void		ParceStr(
		//##Documentation
		//## ����������� ������
		const std::string& src,
		//##Documentation
		//## ������������ ����
	    TINIValue& val);
public:
	//##Documentation
	//## ������ ������ �����
	//##ModelId=41C840A70021
	sects_t		sects;

	//##Documentation
	//## ������ � �������� ����� �����
	//##ModelId=41C840A7003E
	std::string const& FileName();

	//##Documentation
	//## �������� ����������������� ����� �� ���������� ����.
	//## ���� ���������� ������� ����, �� ����� ������������� ����������
	//## TOpenExcept
	//##ModelId=41C840A7003F
	void		Load(
		//##Documentation
		//## ���� � �����.
		const std::string& filename) throw( std::logic_error );

	//##Documentation
	//## �������� ����������������� ����� �� ���������� ����� ����.
	//##ModelId=41C840A70041
	void		Load() throw( std::logic_error ) { Load(fname); };

	//##Documentation
	//## ���������� ����������������� ����� �� ���������� ����.
	//## ���� ���������� ������� ����, �� ����� ������������� ����������
	//## TOpenExcept
	//##ModelId=41C840A7004D
	void		Save(
		//##Documentation
		//## ���� � �����
		const std::string& filename) throw( std::logic_error );

	//##Documentation
	//## ���������� ����������������� ����� �� ����� ���������� ����.
	//##ModelId=41C840A7004F
	void		Flush() throw( std::logic_error ){ Save(fname); };

	//##Documentation
	//## ���������� true ���� ���� ������ � false ���� ���
	//##ModelId=41C840A70050
	bool		KeyExists(
		//##Documentation
		//## ��� ������
		const std::string& section,
		//##Documentation
		//## ��� �����
	    const std::string& key);

	//##Documentation
	//## ���� ������ ���������� ���������� true ����� - false
	//##ModelId=41C840A7005C
	bool		SectionExists(
		//##Documentation
		//## ��� ������
		const std::string& section);

	//##Documentation
	//## ������ ������ ����� � ���� ������.
	//## ���� ���� �/��� ������ �� ����������,
	//## �� ��� ������������� ����� �������.
	//##ModelId=41C840A7005E
	void		WriteInt(
		//##Documentation
		//## ��� ������
		const std::string section,
		//##Documentation
		//## ��� �����
	    const std::string key,
		//##Documentation
		//## ������������ ��������
	    int value);

	//##Documentation
	//## ������ ������ ����� �� �����.
	//## ���� ����� �/��� ������ �� ����������, �� ���� � ������ �����
	//## ������� � ����� ����� ��������� �������� ��-���������.
	//## ���������� �������� �����.
	//##ModelId=41C840A7006D
	int		ReadInt(
		//##Documentation
		//## ��� ������
		const std::string section,
		//##Documentation
		//## ��� �����
	    const std::string key,
		//##Documentation
		//## ������������ �������� "��-���������"
	    int def_value =0 );

	//##Documentation
	//## ������ ������ ����� � ����������������� ����.
	//## ���� ���� �/��� ������ �� ����������,
	//## �� ��� ������������� ����� �������.
	//##ModelId=41C840A70071
	void		WriteHex(const std::string section, const std::string key, int value);

	//##Documentation
	//## ������ �������� ��� ������ ����� ��������������� � �����������������
	//## ����.
	//## ���� ����� �/��� ������ �� ����������, �� ���� � ������ �����
	//## ������� � ����� ����� ��������� �������� ��-���������.
	//## ���������� �������� �����.
	//##ModelId=41C840A7007E
	int		ReadHex(const std::string section, const std::string key, int def_value = 0);

	//##Documentation
	//## ������ ������������� �����
	//## ���� ���� �/��� ������ �� ����������,
	//## �� ��� ������������� ����� �������.
	//##ModelId=41C840A7008B
	void		WriteFloat(const std::string section, const std::string key, float value);

	//##Documentation
	//## ������ ������������� �����
	//## ���� ����� �/��� ������ �� ����������, �� ���� � ������ �����
	//## ������� � ����� ����� ��������� �������� ��-���������.
	//## ���������� �������� �����.
	//##ModelId=41C840A7008F
	float		ReadFloat(const std::string section, const std::string key, float def_value = 0.0);

	//##Documentation
	//## ������ ������
	//## ���� ���� �/��� ������ �� ����������,
	//## �� ��� ������������� ����� �������.
	//##ModelId=41C840A7009C
	void		WriteStr(const std::string section, const std::string key, const std::string value);

	//##Documentation
	//## ������ ������
	//## ���� ����� �/��� ������ �� ����������, �� ���� � ������ �����
	//## ������� � ����� ����� ��������� �������� ��-���������.
	//## ���������� �������� �����.
	//##ModelId=41C840A700A0
	std::string& 	ReadStr(const std::string section, const std::string key,  const std::string def_value = "");

	//##Documentation
	//## ����������� �������
	//##ModelId=41C840A700AC
	TINIFile(
		//##Documentation
		//## ���� � �����.
		std::string filename) : fname(filename) {};

	//##Documentation
	//## ���������� �������.
	//##ModelId=41C840A700AE
	virtual ~TINIFile(){};

};

#endif //_INIFILE_HPP_INCLUDED
