#include <msp430.h> 
#include <stdint.h>
#include <string.h>
/**
 * main.c
 */
//#define     SEND    1
//#define     WAITING    -1
//#define     STOP    0
//#define     WAIT_ALARM      0xAA
//#define     ERROR_ALARM     0xFE
//volatile int state = SEND;
//struct TX_BUF
//{
//    char *buffer;
//    uint16_t size;
//};
//int tmp = 0;
//struct TX_BUF txbuf;
uint8_t time_count=0;
void configClocks(void);
void configIO(void);
void configSPI(void);
void configTimers(void);
void Send_UART(uint8_t data);
//void Send_UART()
void Send_UART_word_ISR(char* word);
void Send_UART_word(char *word);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    configClocks();
    configIO();
    configSPI();
    configTimers();

    __bis_SR_register(GIE );
    while(1)
    {
//        if (SEND == state )

    }
    return 0;
}

void configClocks(void)
{
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;

    BCSCTL1 |= XT2OFF;
    BCSCTL2 |= SELM_1 + DIVM_3 + DIVS_3;
    BCSCTL3 |= LFXT1S_2;
}

void configIO(void)
{
    P1DIR = P2DIR = 0xFF;
    P1OUT = P2OUT = 0xFF;

    P1DIR &= ~BIT3;
    P1REN |= BIT3  ;

    P1IE |= BIT3;
//    P1OUT |= BIT3;
    P1IFG &= ~BIT3;
}

void configSPI(void)
{
    UCA0CTL1 |= UCSWRST;

    UCA0CTL1 |= UCSSEL_2 ;
    UCA0CTL0 |= UCSYNC;
    //UCA0MCTL = UCBRS_1;
    UCA0BR0 = 104;
    UCA0BR1 = 0;


    P1SEL |= BIT1 + BIT2 + BIT4;
    P1SEL2 |= BIT1 + BIT2 + BIT4;

    UCA0CTL1 &= ~UCSWRST;
//    state = SEND;
    IE2 |=  UCA0RXIE;
}
void Send_UART(uint8_t data) {
//    while(UCA0STAT & UCBUSY);
    //trap when Tx is not ready
//    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = data;

}
void Send_UART_word(char *word)
{
    while (*word)
    {
        Send_UART(*word);
        word++;
    }
}
//void Send_UART_word_ISR(char* word)
//{
//    while(!(IFG2 & UCA0TXIFG));
//    txbuf.buffer = word;
//    txbuf.size = strlen(word);
////    IE2 |= UCA0TXIE;
//}
void configTimers(void)
{
    CCTL0 = CCIE;
    CCR0 = 10000;
    TA0CTL = TASSEL_2 + MC_1; // Source by DCO and divide 3 in configClock => 1MHz
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void)
{

    if(time_count < 50)
    {
            time_count++;
    }
    else{
        P1OUT ^= 0x01;
                  // toggle P1.0


        time_count = 0;
    }
    //__bis_SR_register(CPUOFF);
//    __bic_SR_register_on_exit(CPUOFF);
}
#pragma vector = PORT1_VECTOR;
__interrupt void Port1_ISR(void)
{

//    P1OUT ^= 0x01;
    P1OUT |= BIT6;
    Send_UART_word("Testing...\n");
    P1OUT &= ~BIT6;
    P1IFG &= ~BIT3;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI_RX_ISR (void)
{
    if (IFG2 & UCA0RXIFG)
    {
        uint8_t tmp = UCA0RXBUF;
        UCA0TXBUF = 0xAA;
    }
}
