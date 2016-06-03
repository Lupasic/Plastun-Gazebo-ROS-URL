/*
 * ecram.h
 *
 *  Created on: 21.09.2013
 *      Author: user
 */

#ifndef ECRAM_H_
#define ECRAM_H_

typedef int Int;
typedef unsigned int ULONG;

unsigned long ECU_Read_686C_RAM_BYTE(ULONG ECUMemAddr);
unsigned long ECU_Write_686C_RAM_BYTE(ULONG ECUMemAddr, ULONG ECUMemData);

#endif /* ECRAM_H_ */
