/*
 * msp430_spi.h
 *
 *  Created on: Mar 31, 2019
 *      Author: WINNGUYEN
 */

#ifndef MSP430_SPI_H_
#define MSP430_SPI_H_


#include <stdint.h>



void spi_init(void);
uint8_t spi_transfer(uint8_t); //Send character
uint16_t spi_transfer16(uint16_t); //Send number


#endif /* MSP430_SPI_H_ */
