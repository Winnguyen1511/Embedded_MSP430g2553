#include "UART.h"


/**
 * main.c
 */


void sysReset(void);
void numbReset(void);
volatile uint16_t count = 0;
volatile uint8_t decimal;
const uint16_t MAX_SIZE = 3;
volatile uint8_t inputName[MAX_SIZE] = {'a' , 'b' , 'c' };
volatile float input[MAX_SIZE] = {0};

void TXon(void);

void TXoff(void);
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    ConfigClock();
    //Config_IO();
    P1DIR |= BIT0;
    ConfigUART();


    __bis_SR_register(GIE);
    while (1)
    {

        P1OUT |= 0x01;              // toggle P1.0
        _delay_cycles(5000000);
        P1OUT &= 0x00;
        _delay_cycles(5000000);
//        int j;
//        Send_UART_word("Waiting for data from computer!...\n");
//        for (j = 0; j < MAX_SIZE ; j++)
//        {
//            Send_UART(inputName[j]);Send_UART_word(" = ");Send_UART_numb(input[j]);Send_UART('\n');
//        }
    }
    return 0;
}

void sysReset(void)
{
    reset = true;
    next = true;
    done = false;
    count = 0;
    int i;
    for (i = 0; i < MAX_SIZE; i++)
    {
        input[i] = 0;
    }
}

void numbReset(void)
{
    iNumb= 0;
    fNumb = 0;
    pos = 1;
    decimal = 0;
}
void TXon(void)
{
    IE2 |= UCA0TXIE;
}

void TXoff(void)
{
    IE2 &= ~UCA0TXIE;
}
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {

    TXoff();
    unsigned char ch = UCA0RXBUF;
    if(ch != '\n'  )
    {
        if(ch != '\r')
        {
            TXon();
            Send_UART(ch);
            TXoff();
            if (ch == '-') pos = -pos;
            if (ch == '.') decimal = 1;
            if(ch>='0' && ch <= '9')
            {

                if(decimal)
                {
                    fNumb += fNumb *10 + ch -'0';
                    decimal ++;
                }
                else
                {
                    iNumb = iNumb *10 + ch - '0';
                }
            }
        }
        else
        {
            input[count] = pos*(iNumb + (float)fNumb/pow(10, decimal? decimal - 1:decimal));
            numbReset();
            count++;
            next = true;
            TXon();
            Send_UART('\n');
        }
    }
    if (count == MAX_SIZE)
    {
        IE2 &= ~UCA0RXIE;
        done = true;
        next = false;
        TXon();
    }
    //IE2 |= UCA0TXIE;
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void){

    if (reset)
    {
         Send_UART_word("Please enter your input: \n");
         reset = false;

    }
    if(next)
    {
        Send_UART(inputName[count]);Send_UART_word(" = ");
        next = false;

        TXoff();
        IE2 |= UCA0RXIE;
    }
    if (done)
    {
        uint16_t j;
        Send_UART_word("Your input are: \n");
        for (j = 0; j < MAX_SIZE ; j++)
        {
            Send_UART(inputName[j]);Send_UART_word(" = ");Send_UART_numb(input[j]);Send_UART('\n');
        }
        sysReset();
        IE2 &= ~UCA0RXIE;
        TXoff();
    }

    TXon();

}
