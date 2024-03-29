#include <msp430.h> 
#include <stdint.h>
#include <string.h>
#include "Queue.h"

/**
 * main.c
 */
#define     RECORDING   1
#define     WAIT        0
#define     FULL        -1
#define     FULL_ALARM  0xAA
struct TX_BUF
{
    char *buffer;
    uint16_t size;
};
struct TX_BUF txbuf;
volatile    int     state = RECORDING;
uint8_t time_count = 0;
struct Queue rxbuf;
void configClocks(void);
void configIO(void);
void configSPI(void);
void configTimers(void);
void Send_UART(uint8_t data);
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    configClocks();
    configIO();
    configSPI();
    configTimers();
    createQueue(&rxbuf);
//    __no_operation();
    __bis_SR_register(GIE);
    while(1);
    return 0;
}

void configClocks(void)
{
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;

    //BCSCTL1 |= XT2OFF;
    BCSCTL2 |= SELM_1 + DIVM_3+DIVS_3;
    //BCSCTL3 |= LFXT1S_2;
}

void configIO(void)
{
    P1DIR = P2DIR = 0xFF;
    P1OUT = P2OUT = 0xFF;

    P1DIR &= ~(+BIT3);
    P1IE |= BIT3;
    P1IES |= BIT3;
    P1IFG &= ~BIT3;
    P1REN |= BIT3;
//    P2IE |= BIT0;
//    P1OUT |= BIT3;
}

void configSPI(void)
{
    //UART
    UCA0CTL1 |= UCSWRST;
//    UCA0CTL0 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;

    UCA0MCTL = UCBRS_1;
    UCA0BR0 = 104;
    UCA0BR1 = 0;

    P1SEL |= BIT1 + BIT2 + BIT4;
    P1SEL2 |= BIT1 + BIT2 + BIT4;


    UCA0CTL1 &= ~UCSWRST;

//    UCA0CTL0 &= ~UCSWRST;


    //UCB config
    UCB0CTL1 |= UCSWRST;

//    UCB0CTL1 |= UCSSEL_2 ;
    UCB0CTL0 |=  UCSYNC;
    //UCA0MCTL = UCBRS_1;
    UCB0BR0 = 104;
    UCB0BR1 = 0;


    P1SEL |= BIT6 + BIT7 + BIT5;
    P1SEL2 |= BIT6 + BIT7 + BIT5;

    UCB0CTL1 &= ~UCSWRST;
    IE2 |= UCB0RXIE  + UCA0TXIE;
}
void Send_UART(uint8_t data) {
    while(!(IFG2 & UCA0TXIFG));
//    while(UCA0STAT & UCBUSY);
    UCA0TXBUF = data;
}

void Send_UART_word(char* word)
{
//    while(! (IFG2 & UCA0TXIFG) );
//    IFG2 |= UCA0TXIFG;
    txbuf.buffer = word;
    txbuf.size = strlen(word);
    while(txbuf.size--)
    {
        UCA0TXBUF = *txbuf.buffer++;
    }
}
void configTimers(void)
{
    CCTL0 = CCIE;
    CCR0 = 10000;
    TA0CTL = TASSEL_2 + MC_1; // Source by DCO and divide 3 in configClock => 1MHz
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
//    if (WAIT == state ) state = RECORDING;


    if(isFull(rxbuf))
    {
        state = FULL;
        UCB0TXBUF = FULL_ALARM;
    }
    else
    {   state = RECORDING;
        uint8_t rxtmp = UCB0RXBUF;
        enqueue(&rxbuf,rxtmp);
    }


}
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
//    if(txbuf.size)
//    {
//        UCA0TXBUF = *txbuf.buffer;
//        txbuf.buffer++;
//        txbuf.size--;
//    }
//    else
//    {
//        //Wait for new data to send !!
//    }
    uint8_t txtmp = 0;
//    if(FULL == state)
//        UCB0TXBUF = FULL_ALARM;

    flag:
        if(WAIT != state)
        {

            if(isEmpty(rxbuf))
            {
                state = WAIT;
                goto flag;
            }
            dequeue(&rxbuf,&txtmp);
//            while(!(IFG2 & UCA0TXIFG));
            UCA0TXBUF = txtmp;
//            IFG2 |= UCA0TXIFG;
        }
        else
        {
//            state = RECORDING;
        }
//    if(IFG2 & UCA0TXIFG)
//        UCA0TXBUF = txtmp;

}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void)
{
    if(time_count < 20) time_count++;
    else{
        P1OUT ^= 0x01;
        time_count = 0;
    }// toggle P1.0
//    Send_UART_word("hello world !\n");

    //__bis_SR_register(CPUOFF);
//    __bic_SR_register_on_exit(CPUOFF);
}
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
    IE2 &= ~UCA0TXIE;

    Send_UART_word("Waiting for data from master!...\n");

    state = WAIT;
    IE2 |= UCA0TXIE;
//    IE2 &= ~UCA0TXIE;
    P1IFG &= ~BIT3;

}
//#pragma vector =PORT2_VECTOR
//__interrupt void Port2_ISR(void)
//{
//    state = REQUEST;
//}
