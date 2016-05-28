#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "inifile.h"

//##ModelId=41C840A700FB
void	TINIValue::IntValue(int val)
{
	std::ostringstream	os;
	os << val;
	value = os.str();
}

//##ModelId=41C840A700FD
int	TINIValue::IntValue()
{
	return atoi(value.c_str());
}

//##ModelId=41C840A700FE
void	TINIValue::HexValue(int val)
{
	std::ostringstream	os;
	os << "0x" << std::hex << val;
	value = os.str();
}

//##ModelId=41C840A70108
int	TINIValue::HexValue()
{
	std::istringstream is(value);
	int res;
	is >> std::hex >> res;
	return res;
}

//##ModelId=41C840A70109
void	TINIValue::StrValue(const std::string& val)
{
	value = val;
}

//##ModelId=41C840A7010B
std::string&	TINIValue::StrValue()
{
	return value;
}



//##ModelId=41C840A7010C
void	TINIValue::FloatValue(float val)
{
	std::ostringstream	os;
	os << val;
	value = os.str();
}

//##ModelId=41C840A7010E
float	TINIValue::FloatValue()
{
	return atof(value.c_str());
}


//##ModelId=41C840A70025
std::string&	TINIFile::TrimStr(std::string& str)
{
	//Trimming comments
	size_t erase_to = 0;
	size_t erase_from = str.find(';');
	if(erase_from < str.npos)
		str.erase( erase_from );
	//Trimming left side of std::string from spaces and tabs
	std::string dlm(" \t\r\n");//configure delimiters std::string
	erase_from = 0;
	erase_to = str.find_first_not_of(dlm);//Let's find first not delimiter symbol
	if( erase_to != str.npos )
		str.erase( erase_from, erase_to);
	//Trimming right side of std::string from spaces and tabs
	erase_from = str.find_last_not_of(dlm)+1;//Let's find last not delimiter symbol
	if( erase_from < str.npos )
		str.erase( erase_from );

	return str;
}

//Determining is std::string a section
//##ModelId=41C840A7002E
bool	TINIFile::IsSection(const std::string& str)
{
	if ( (str[0] == '[') && (str[str.length() - 1] == ']') )
		return true;
	else
		return false;
}


//##ModelId=41C840A70030
void	TINIFile::ParceStr(const std::string& src, TINIValue& val)
{
	size_t copy_to		= src.find('=');
	if(copy_to < src.npos)
	{
		size_t copy_from	= copy_to + 1;
		val.Name() = src.substr(0, copy_to);
		val.Value() = src.substr(copy_from);
		
		TrimStr( val.Name() );
		TrimStr( val.Value() );
	}
	else
	{
		val.Name() = "";
		val.Value() = "";
	}
}

//##ModelId=41C840A7003F
void	TINIFile::Load(const std::string& filename) throw( std::logic_error )
{
	std::ifstream input(filename.c_str());
	if( !input)
		throw std::logic_error( "TINIFile::Load() exception" );
	fname = filename;
	sects_t::iterator cur = sects.begin();
	TINIValue item("","");
	while(!input.eof())
	{
		std::string buf;
		char c = input.get();
		while( (c != 13) && (c != 10) && (!input.eof()) )
		{
			buf+=c;
			c = input.get();
		}
		TrimStr(buf);
		//If trimmed std::string less than 3 characters let's pass it thru
		if(buf.length() < 3) continue;
		if( IsSection(buf) )
		{
			std::string aName( buf.substr(1, buf.length()-2) );
			TrimStr( aName );
			if( sects.find( aName ) == sects.end())
				sects[ aName ].Name() = aName;
			cur = sects.find( aName );
		}
		else
		if(cur != sects.end())
		{
			ParceStr(buf, item);
			//If key and value is good then
			if( !item.Name().empty() )
				cur->second.items[item.Name()] = item;
		}
	}
};




//##ModelId=41C840A7004D
void	TINIFile::Save(const std::string& filename) throw( std::logic_error )
{
	std::ofstream output( filename.c_str() );
	if( !output)
		throw std::logic_error( "TINIFile::Save() exception" );
	fname = filename;
	sects_t::iterator cur = sects.begin();
	output << "; INI file syntax:" << std::endl;
	output << "; [%section_name%] - config section" << std::endl;
	output << "; %value_key%=%value% - config key and it's value" << std::endl;
	time_t t = std::time(0);
	output << "; This file was modified  @ " << std::string(std::ctime(&t)) << std::endl;
	while(cur!=sects.end())
	{
		output << '[' << cur->first << "]" << std::endl;
		items_t::iterator val = cur->second.items.begin();
		while( val != cur->second.items.end() )
		{
			output << val->first << '=' << val->second.Value() << std::endl;
			val++;
		}
		cur++;
	}
}

//##ModelId=41C840A70050
bool TINIFile::KeyExists(const std::string& section, const std::string& key)
{
	if(SectionExists(section))
		if( sects[section].items.find(key) != sects[section].items.end() )
			return true;
	return false;
}

//##ModelId=41C840A7005C
bool	TINIFile::SectionExists(const std::string& section)
{
	return ( sects.find(section) != sects.end() );
}

//##ModelId=41C840A7005E
void TINIFile::WriteInt(const std::string section, const std::string key, int value)
{
	sects[section].Name() = section;
	sects[section].items[key].IntValue(value);
}

//##ModelId=41C840A7006D
int	TINIFile::ReadInt(const std::string section, const std::string key, int def_value )
{
	sects[section].Name() = section;
	if( !KeyExists(section, key) )
		sects[section].items[key].IntValue(def_value);
	return sects[section].items[key].IntValue();
}

//##ModelId=41C840A70071
void TINIFile::WriteHex(const std::string section, const std::string key, int value)
{
	sects[section].Name() = section;
	sects[section].items[key].HexValue(value);
}

//##ModelId=41C840A7007E
int	TINIFile::ReadHex(const std::string section, const std::string key, int def_value)
{
	sects[section].Name() = section;
	if( !KeyExists(section, key) )
		sects[section].items[key].HexValue(def_value);
	return sects[section].items[key].HexValue();
}

//##ModelId=41C840A7008B
void	TINIFile::WriteFloat(const std::string section, const std::string key, float value)
{
	sects[section].Name() = section;
	sects[section].items[key].FloatValue(value);
}

//##ModelId=41C840A7008F
float	TINIFile::ReadFloat(const std::string section, const std::string key, float def_value )
{
	sects[section].Name() = section;
	if( !KeyExists(section, key) )
		sects[section].items[key].FloatValue(def_value);
	return sects[section].items[key].FloatValue();
}

//##ModelId=41C840A7009C
void	TINIFile::WriteStr(const std::string section, const std::string key, const std::string value)
{
	sects[section].Name() = section;
	sects[section].items[key].StrValue(value);
}

//##ModelId=41C840A700A0
std::string& TINIFile::ReadStr(const std::string section, const std::string key, const std::string def_value )
{
	sects[section].Name() = section;
	if( !KeyExists(section, key) )
		sects[section].items[key].StrValue(def_value);
	return sects[section].items[key].StrValue();
}
//##ModelId=41C840A7003E
std::string const& TINIFile::FileName()
{
    return fname;
}
