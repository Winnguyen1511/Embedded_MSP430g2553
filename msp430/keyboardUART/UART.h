/*
 * UART.h
 *
 *  Created on: Sep 15, 2015
 *      Author: JuryTare
 */

#ifndef UART_H_
#define UART_H_

#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void Config_Clock();
void Config_UART();
void Send_UART(uint8_t);
void Send_UART_word(char *);
void Send_UART_numb(uint16_t);

#endif
