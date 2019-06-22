#include <msp430.h> 
#include <stdint.h>

#include "msp430_spi.h"
#include "msprf24.h"

/**
 * main.c
 */
volatile uint16_t user;
void configClocks(void);
void configNrf24(void);
void configIO(void);
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	configClocks();
	configIO();
	configNrf24();
	uint8_t addr[5];
	uint8_t buf[32];
	user = 0xFE;
	addr[0] = 0xDE; addr[1] = 0xAD; addr[2] = 0xBE; addr[3] = 0xEF; addr[4] = 0x00;
	w_rx_addr(0,addr);
	if (!(RF24_QUEUE_RXEMPTY & msprf24_queue_state())) {
        flush_rx();
	}
	msprf24_activate_rx();
	while (1) {
	        if (rf_irq & RF24_IRQ_FLAGGED) {
	            rf_irq &= ~RF24_IRQ_FLAGGED;
	            msprf24_get_irq_reason();
	        }
	        if (rf_irq & RF24_IRQ_RX || msprf24_rx_pending()) {
	            r_rx_payload(32, buf);
	            msprf24_irq_clear(RF24_IRQ_RX);
	            user = buf[0];

	            if (buf[0] == '0')
	                P1OUT &= ~BIT0;
	            if (buf[0] == '1')
	                P1OUT |= BIT0;
	            if (buf[1] == '0')
	                P1OUT &= ~BIT6;
	            if (buf[1] == '1')
	                P1OUT |= BIT6;

	        } else {
	            user = 0xFF;
	        }

	    }
	return 0;
}

void configClocks(void)
{
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;
    BCSCTL2 |= SELM_1 + DIVS_3 + DIVM_3;

}

void configIO(void)
{
    P1DIR |= BIT0 + BIT6;
    P1OUT &= ~(BIT0 + BIT6);

}
void configNrf24(void)
{
    /* Initial values for nRF24L01+ library config variables */
    rf_crc = RF24_EN_CRC | RF24_CRCO; // CRC enabled, 16-bit
    rf_addr_width      = 5;
    rf_speed_power     = RF24_SPEED_1MBPS | RF24_POWER_0DBM;
    rf_channel         = 120;
    msprf24_init();
    msprf24_set_pipe_packetsize(0, 32);
    msprf24_open_pipe(0, 1);  // Open pipe#0 with Enhanced ShockBurst


}
