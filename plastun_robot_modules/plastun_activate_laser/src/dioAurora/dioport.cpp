//============================================================================
// Name        : test_port.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/io.h>
#include <fcntl.h>
#include <stdint.h>
#include "ecram.h"
using namespace std;


#define DIO_P0_DIR 0x1658
#define DIO_P1_DIR 0x1659
#define DIO_P2_DIR 0x165A
#define DIO_P3_DIR 0x165B
#define DIO_P4_DIR 0x165C
#define DIO_P5_DIR 0x1614
#define DIO_P6_DIR 0x1615
#define DIO_P7_DIR 0x1616

#define DIO_P0_LVL 0x160A
#define DIO_P1_LVL 0x160A
#define DIO_P2_LVL 0x160A
#define DIO_P3_LVL 0x160A
#define DIO_P4_LVL 0x160A
#define DIO_P5_LVL 0x1601
#define DIO_P6_LVL 0x1601
#define DIO_P7_LVL 0x1601

#define DIO_P0_BIT 0
#define DIO_P1_BIT 1
#define DIO_P2_BIT 2
#define DIO_P3_BIT 3
#define DIO_P4_BIT 4
#define DIO_P5_BIT 4
#define DIO_P6_BIT 5
#define DIO_P7_BIT 6

#define LASER_ON   0x20 //6 bit on
#define LASER_OFF_MASK  0xDF //no 6 bit
#define OUTPUT	 0x40

short int dir_port[8] = {DIO_P0_DIR, DIO_P1_DIR, DIO_P2_DIR, DIO_P3_DIR ,DIO_P4_DIR ,
			DIO_P5_DIR, DIO_P6_DIR, DIO_P7_DIR};

short int lvl_port[8] = {DIO_P0_LVL, DIO_P1_LVL, DIO_P2_LVL, DIO_P3_LVL, DIO_P4_LVL,
		DIO_P5_LVL, DIO_P6_LVL,DIO_P7_LVL};

uint8_t dio_mask[8] = { 1<<DIO_P0_BIT, 1<<DIO_P1_BIT, 1<<DIO_P2_BIT, 1<<DIO_P3_BIT,
		1<<DIO_P4_BIT, 1<<DIO_P5_BIT, 1<<DIO_P6_BIT, 1<<DIO_P7_BIT};


bool initPorts()
{
	if (iopl( 3))
	{
		std::cout << " not permitted " << std::endl;
		return false;
	}
	return true;
}

void write_port(uint8_t val, int port)
{

	std::cout<<" write val 0x"<<std::hex<<(int)val<<std::dec<<std::endl;
	int res = ECU_Write_686C_RAM_BYTE(port, val);
	std::cout<<"res = "<<res<<std::endl;
}

uint8_t read_port(int port)
{

	uint8_t val(0);
	val = ECU_Read_686C_RAM_BYTE(port );
	std::cout<<" val = 0x"<<std::hex<<(int)val<<std::dec<<std::endl;
//	if (res != 1)
//		return -1;
	return val;
}


void setOut(int ch)
{
	std::cout<<"set out 0x"<<std::hex<<(int)dir_port[ch]<<std::dec<<std::endl;
	read_port(dir_port[ch]);
	uint8_t val = OUTPUT;

	write_port(val,dir_port[ch] );

	read_port(dir_port[ch]);
}

void setHigh(int ch)
{
	std::cout<<"set high 0x"<<std::hex<<(int)lvl_port[ch]<<std::dec<<std::endl;
	uint8_t val = read_port(lvl_port[ch] );

	val |= dio_mask[ch];
	write_port(val, lvl_port[ch]  );

	val = read_port(lvl_port[ch] );

}

void setLow(int ch)
{
	std::cout<<"set high 0x"<<std::hex<<(int)lvl_port[ch]<<std::dec<<std::endl;
	uint8_t val = read_port(lvl_port[ch] );

	val &= ( ~dio_mask[ch]);
	write_port(val, lvl_port[ch] );

	val = read_port(lvl_port[ch] );
}


