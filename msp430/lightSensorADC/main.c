#include <msp430.h> 


/**
 * main.c
 */
volatile long tempRaw;
void configClocks(void);
void configIO(void);
void configUART(void);
void configTimerA(void);
void configADC10(void);
//void Send_UART_numb(uint16_t);
//void Send_UART_word(uint8_t*);
//void Send_UART(uint8_t);
int main(void)
{
    configClocks();
    configIO();
//    configUART();
    configTimerA();
    configADC10();

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	__bis_SR_register( GIE);

	return 0;
}
//void Send_UART(int8_t data) {
//    while(!(IFG2 & UCA0TXIFG));
//    UCA0TXBUF = data;
//
//}
//
//void Send_UART_word(char* word)
//{
//    while(*word) {
//        Send_UART(*word++);
////        word ++; // Lay ky tu tiep theo
//    }
//}
//
//void Send_UART_numb(float number) {
//    char data[8];
//    ftoa(number, data,2);
//    Send_UART_word(data);
//}
//void configUART(void)
//{
//    UCA0CTL1 |= UCSWRST;
//    UCA0CTL1 |= UCSSEL_2;
//
//    UCA0MCTL = UCBRS_1;//9600baud
//    UCA0BR0 = 104;
//    UCA0BR1 = 0;
//
//    P1SEL |= BIT1 + BIT2;
//    P1SEL2 |= BIT1 + BIT2;
//
//    UCA0CTL1 &= ~UCSWRST;
//    IE2 |= UCA0RXIE;
//}
void configClocks(void)
{
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;
    BCSCTL1 |= XT2OFF;
    BCSCTL2 |= SELM_1 +  DIVM_3 + DIVS_3;
    BCSCTL3 |= LFXT1S_2;
}

void configIO(void)
{
    P1DIR = BIT0 ;
//    P1SEL |= BIT0 + BIT4;
//    P1SEL2 |= BIT0 + BIT4;



}
void configADC10(void)
{
    ADC10CTL1 = INCH_10 + ADC10SSEL_3 + ADC10DIV_7;
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON;
    __delay_cycles(50);
    ADC10CTL0 |= ENC + ADC10SC;
    __delay_cycles(500);
    ADC10CTL0 &= ~ENC; // Disable ADC conversion
    ADC10CTL0 &= ~(REFON + ADC10ON); //Ref and ADC10 off
    tempRaw = ADC10MEM;
}

void configTimerA(void)
{
    CCTL0 = CCIE;
    CCR0 = 1000000;
    TA0CTL = TASSEL_2 + MC_2; // Source by DCO and divide 3 in configClock => 1MHz
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void)
{
//    __bic_SR_register_on_exit(CPUOFF);
    ADC10CTL0 = SREF_1 + ADC10SHT_0 + REFON + ADC10ON;
    __delay_cycles(50);
    ADC10CTL0 |= ENC + ADC10SC;
    __delay_cycles(500);
    ADC10CTL0 &= ~ENC; // Disable ADC conversion
    ADC10CTL0 &= ~(REFON + ADC10ON); //Ref and ADC10 off
    tempRaw = ADC10MEM;
    P1OUT ^= BIT0;
                  // toggle P1.0
    CCR0 +=1000000;

//    _delay_cycles(1000000);
}
