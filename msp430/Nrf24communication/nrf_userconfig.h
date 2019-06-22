/*
 * nrf_userconfig.h
 *
 *  Created on: Mar 31, 2019
 *      Author: WINNGUYEN
 */

#ifndef NRF_USERCONFIG_H_
#define NRF_USERCONFIG_H_

// Setting delay time for 1MHz

#define DELAY_CYCLES_5MS    5000
#define DELAY_CYCLES_130US  130
#define DELAY_CYCLES_15US   15

// Config USCI master(USCI_A) or slave (USCI_B)
#define SPI_DRIVER_USCI_A   1
//#define SPI_DRIVER_USCI_B   1

// Config Port for NRF24
/* IRQ */
#define nrfIRQport 2
#define nrfIRQpin BIT2

/* CSN SPI chip-select */
#define nrfCSNport 2
#define nrfCSNportout P2OUT
#define nrfCSNpin BIT1

/* CE Chip-Enable (used to put RF transceiver on-air for RX or TX) */
#define nrfCEport 2
#define nrfCEportout P2OUT
#define nrfCEpin BIT0

#endif /* NRF_USERCONFIG_H_ */
