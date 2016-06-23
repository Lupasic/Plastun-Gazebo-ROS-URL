/*
 * dioport.h
 *
 *  Created on: 21.09.2013
 *      Author: user
 */

#ifndef DIOPORT_H_
#define DIOPORT_H_
#include <stdint.h>

bool initPorts();
void write_port(uint8_t val, int port);
uint8_t read_port(int port);
void setOut(int ch);
void setHigh(int ch);
void setLow(int ch);

#endif /* DIOPORT_H_ */
