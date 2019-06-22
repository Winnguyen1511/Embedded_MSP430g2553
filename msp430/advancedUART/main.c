#include "Queue.h"
//#include "UART.h"
#define     TIMER_COUNT_8BIT    8
#define     TIMER_COUNT_4BIT    4
uint8_t time_count = 0;
struct Queue txbuf;
void Config_IO(void);
void Config_Clock(void);
void Config_UART(void);
void Config_Timer(void);
void Send_UART(uint8_t);
void Send_UART_ISR(uint8_t);
void Send_UART_word_ISR(char*);
void Send_UART_word(char *);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

        Config_Clock();
        Config_IO();
        Config_UART();
        Config_Timer();

        __bis_SR_register(GIE);
        txbuf = createQueue(8);
        while(1) {
            while(P1IN & BIT3) {
                P1OUT ^= BIT6;
                __delay_cycles(1000000);
            }
//            __delay_cycles(1000000);
            P1OUT |= BIT0;
            Send_UART_word_ISR("hello world\n");
            P1OUT &= ~BIT0;

        }
    return 0;
}
void Config_Timer(void)
{
    CCTL0 |= CCIE;
    CCR0 = 32;
    TA0CTL |= TASSEL_2;
}
void Config_IO() {
    P1DIR = P2DIR = 0xFF;
    P1OUT = P2OUT = 0xFF;

    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;
}
void Config_Clock() {
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;

    BCSCTL1 |= XT2OFF;
    BCSCTL2 |= SELM_1 + DIVS_3;
    BCSCTL3 |= LFXT1S_2;
}

void Config_UART() {
        UCA0CTL1 |= UCSWRST;

        UCA0CTL1 |= UCSSEL_2 ;

        UCA0MCTL = UCBRS_1;
        UCA0BR0 = 104;
        UCA0BR1 = 0;

        P1SEL |= BIT1 + BIT2;
        P1SEL2 |= BIT1 + BIT2;

        UCA0CTL1 &= ~UCSWRST;

        //IE2 |= UCA0RXIE;
        //IE2 |= UCA0TXIE;

}

void Send_UART_ISR(uint8_t data)
{
        enqueue(&txbuf, data);

}
void Send_UART_word_ISR(char* word)
{
    while(*word)
    {
        if(isFull(&txbuf))
        {
            IE2|= UCA0TXIE;
        }
        else{
        Send_UART_ISR(*word);
        word++;}
    }
    TA0CTL |= MC1;
}
void Send_UART(uint8_t data) {
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = data;
}

void Send_UART_word(char* word)
{
    while(*word) {
        Send_UART(*word);
        word ++; // Lay ky tu tiep theo
    }
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA0_ISR(void)
{
    if(TIMER_COUNT_4BIT > time_count)
        time_count++;
    else
    {
        time_count = 0;
        TA0CTL &= ~MC1;
        IE2 |= UCA0TXIE;
    }

}
#pragma vector = USCIAB0TX_VECTOR
__interrupt void UCA0TX_ISR(void)
{
    char newData = 0;
    while(SUCCESS_KEY == dequeue(&txbuf, &newData))
    {
        Send_UART(newData);
    }
    IE2 &= ~UCA0TXIE;
}
