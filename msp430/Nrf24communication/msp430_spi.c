/*
 * msp430_spi.c
 *
 *  Created on: Mar 31, 2019
 *      Author: WINNGUYEN
 */



#include <msp430.h>
#include "nrf_userconfig.h"
#include "msp430_spi.h"
#if defined(__MSP430_HAS_USCI__) && defined(SPI_DRIVER_USCI_A) && !defined(__MSP430_HAS_TB3__)
void spi_init (void)
{
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2; //SCLK
    UCA0CTL0 |= UCMST + UCSYNC;
    P1SEL |= BIT1 + BIT2 + BIT4;
    P1SEL2 |= BIT1 + BIT2 + BIT4;
    UCA0BR0 = 104;
    UCA0BR1 = 0;
    UCA0CTL1 &= ~UCSWRST;
}

uint8_t spi_transfer(uint8_t data)
{
    UCA0TXBUF = data;
    while(!(IFG2 & UCA0RXIFG));
    return UCA0RXBUF;
}
uint16_t spi_transfer16(uint16_t numb)
{
    uint16_t rxtmp16;
    uint8_t *rxtmp8 = (uint8_t *)&rxtmp16;
    uint8_t *txtmp8 = (uint8_t *)&numb;

    UCA0TXBUF = txtmp8[1];
    while (!(IFG2 & UCA0RXIFG));
    rxtmp8[1] = UCA0RXBUF;

    UCA0TXBUF = txtmp8[0];
    while (!(IFG2 & UCA0RXIFG));
    rxtmp8[0] = UCA0RXBUF;

    return rxtmp16;
}
#endif

#if defined(__MSP430_HAS_USCI__) && defined(SPI_DRIVER_USCI_B) && !defined(__MSP430_HAS_TB3__)
void spi_init()
{
    /* Configure ports on MSP430 device for USCI_B */
    P1SEL |= BIT5 | BIT6 | BIT7;
    P1SEL2 |= BIT5 | BIT6 | BIT7;

    /* USCI-B specific SPI setup */
    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC;  // SPI mode 0, master
    UCB0BR0 = 0x01;  // SPI clocked at same speed as SMCLK
    UCB0BR1 = 0x00;
    UCB0CTL1 = UCSSEL_2;  // Clock = SMCLK, clear UCSWRST and enables USCI_B module.
}

uint8_t spi_transfer(uint8_t inb)
{
    UCB0TXBUF = inb;
    while ( !(IFG2 & UCB0RXIFG) )  // Wait for RXIFG indicating remote byte received via SOMI
        ;
    return UCB0RXBUF;
}

uint16_t spi_transfer16(uint16_t inw)
{
    uint16_t retw;
    uint8_t *retw8 = (uint8_t *)&retw, *inw8 = (uint8_t *)&inw;

    UCB0TXBUF = inw8[1];
    while ( !(IFG2 & UCB0RXIFG) )
        ;
    retw8[1] = UCB0RXBUF;
    UCB0TXBUF = inw8[0];
    while ( !(IFG2 & UCB0RXIFG) )
        ;
    retw8[0] = UCB0RXBUF;
    return retw;
}
#endif
