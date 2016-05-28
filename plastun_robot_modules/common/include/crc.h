/******************************************************************************
 * CRC calculation module. This is the part of QNX Classes library project.
 * Created at 30.11.2004
 * 
 * (c) by Khandrikov Sergei 2004 - 2005.
 * 
 * Note:
 * 	This module is based on sources and texts published in SU.FIDOTECH by
 * 	Dmitry Tomashpolski, 2:5030/163.167@fidonet, toddy@mail.ru    03.04.1999
 * 	Greatest respect to U, Dmitry!!!
 * 
 ******************************************************************************/
#ifndef CRC_H_HEADER_INCLUDED_BE534F61
	#define CRC_H_HEADER_INCLUDED_BE534F61


#include <stdint.h>
#include <unistd.h>

// Абстрактный класс вычисления CRC
//##ModelId=41F2747B030D
class TCRC 
{
public:
	// Обновление CRC суммы
	//##ModelId=41F2747B034C
	virtual void Update(
		// Буфера данных для обработки
		const uint8_t* buf, 
		// Размер передаваемого буфера в байтах
		const size_t& size) = 0;

	// Очистка CRC
	//##ModelId=41F2747B037B
	virtual void Clear() = 0;

};

// Абстрактный класс вычисления CRC-16
//##ModelId=41F2747B031C
class TCRC16 : public TCRC
{
protected:
	// контейнер CRC суммы
	//##ModelId=41AC82870128
	uint16_t crc;

public:
	//##ModelId=41F2747B03C8
	uint16_t get_crc() const;

	//##ModelId=41F2747B03D8
	operator uint16_t();
	//##ModelId=41F2747B03D9
	TCRC16();

	//##ModelId=41F2747B03DA
	virtual ~TCRC16();

	//##ModelId=41F2747B03DC
	int operator==(const TCRC16& right);

	//##ModelId=41F2747B03DE
	int operator!=(const TCRC16& right);
	//##ModelId=41F2747B03E7
	virtual void Clear();


};

// LHA CRC-16
//##ModelId=41F2747B032C
class TCRC16_LHA : public TCRC16
{
protected:
	// инициализация данных
	//##ModelId=41F2747C003D
	void init();

	// Таблица для вычисления CRC
	//##ModelId=41AC837703D7
	static uint16_t* tb16;
	// Количество существующих объектов класса
	//##ModelId=41AC89AD02ED
	static unsigned count;

public:
	// Обновление CRC суммы
	//##ModelId=41F2747C003E
	virtual void Update(
		// Буфера данных для обработки
		const uint8_t* buf, 
		// Размер передаваемого буфера в байтах
		const size_t& size);


	//##ModelId=41F2747C0042
	TCRC16_LHA();

	//##ModelId=41F2747C0043
	TCRC16_LHA(const TCRC16_LHA& right);

	//##ModelId=41F2747C0045
	virtual ~TCRC16_LHA();
	// Конструктор с параметрами
	//##ModelId=41F2747C004E
	TCRC16_LHA(
		// Буфер для анализа
		const uint8_t* buf, 
		// Размер передаваемого буфера
		const size_t& size);


};

// Z-Modem CRC-16
//##ModelId=41F2747B033B
class TCRC16_Z_modem : public TCRC16
{
protected:
	// инициализация данных
	//##ModelId=41F2747C005E
	void init();

	//##ModelId=41AC879703E7
	static uint16_t* tb16;

	// Количество существующих объектов класса
	//##ModelId=41AC8D690280
	static unsigned count;

public:
	// Обновление CRC суммы
	//##ModelId=41F2747C005F
	virtual void Update(
		// Буфера данных для обработки
		const uint8_t* buf, 
		// Размер передаваемого буфера в байтах
		const size_t& size);

	//##ModelId=41F2747C0063
	TCRC16_Z_modem();

	//##ModelId=41F2747C0064
	TCRC16_Z_modem(const TCRC16_Z_modem& right);

	//##ModelId=41F2747C006D
	virtual ~TCRC16_Z_modem();

	// Конструктор с параметрами
	//##ModelId=41F2747C006F
	TCRC16_Z_modem(
		// Буфер для анализа
		const uint8_t* buf, 
		// Размер передаваемого буфера
		const size_t& size);

};

// CCITT CRC-16
//##ModelId=41F2747B032B
class TCRC16_CCITT : public TCRC16
{
protected:
	// инициализация данных
	//##ModelId=41F2747C000F
	void init();

	//##ModelId=41AC905F03A9
	static uint16_t* tb16;

	// Количество существующих объектов класса
	//##ModelId=41AC905F03B8
	static unsigned count;

public:
	// Обновление CRC суммы
	//##ModelId=41F2747C0010
	virtual void Update(
		// Буфера данных для обработки
		const uint8_t* buf, 
		// Размер передаваемого буфера в байтах
		const size_t& size);

	//##ModelId=41F2747C0014
	TCRC16_CCITT();

	//##ModelId=41F2747C0015
	TCRC16_CCITT(const TCRC16_Z_modem& right);

	//##ModelId=41F2747C001F
	virtual ~TCRC16_CCITT();

	// Конструктор с параметрами
	//##ModelId=41F2747C0021
	TCRC16_CCITT(
		// Буфер для анализа
		const uint8_t* buf, 
		// Размер передаваемого буфера
		const size_t& size);

};

// CCITT CRC-32
//##ModelId=41F2747B033C
class TCRC32 : public TCRC
{
protected:
	//##ModelId=41F2747C007E
	void init();

	// переменная, содержащая CRC32
	//##ModelId=41AC9266036A
	uint32_t crc;

	// Таблице вычисления CRC32
	//##ModelId=41AC929303D7
	static uint32_t *tb32;

	// Количество объектов класса
	//##ModelId=41AC92C0031C
	static unsigned count;

public:
	//##ModelId=41F2747C008C
	uint32_t get_crc() const;

	//##ModelId=41F2747C008E
	virtual void Clear();

	//##ModelId=41F2747C0090
	operator uint32_t();
	// Обновление CRC суммы
	//##ModelId=41F2747C0091
	virtual void Update(
		// Буфера данных для обработки
		const uint8_t* buf, 
		// Размер передаваемого буфера в байтах
		const size_t& size);


	//##ModelId=41F2747C009C
	int operator==(const TCRC32& right);

	//##ModelId=41F2747C009E
	int operator!=(const TCRC32& right);

	//##ModelId=41F2747C00A0
	TCRC32();

	//##ModelId=41F2747C00A1
	TCRC32(const TCRC32& right);

	// Конструктор с параметрами
	//##ModelId=41F2747C00A3
	TCRC32(
		// Буфер для анализа
		const uint8_t* buf, 
		// Размер передаваемого буфера
		const size_t& size);

	//##ModelId=41F2747C00AB
	virtual ~TCRC32();

};



#endif /* CRC_H_HEADER_INCLUDED_BE534F61 */
