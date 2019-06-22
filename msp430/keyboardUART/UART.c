/*
 * UART.c
 *
 *  Created on: Feb 14, 2019
 *      Author: WINNGUYEN
 */
#include "UART.h"




void ConfigClock(void)
{
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;
    BCSCTL1 |= XT2OFF;
    BCSCTL2 |= SELM_1 +  DIVS_3;
    BCSCTL3 |= LFXT1S_2;
}

void ConfigUART(void)
{
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;

    UCA0MCTL = UCBRS_1;//9600baud
    UCA0BR0 = 104;
    UCA0BR1 = 0;

    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;

    UCA0CTL1 &= ~UCSWRST;
    //IE2 |= UCA0RXIE;
    //IE2 |= UCA0RXIE ;
    IE2 |= UCA0RXIE;
//    numbReset();
//    reset = true;
//    next = true;
//    done = false;
//    numbReset();
}

void Send_UART(int8_t data) {
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = data;

}

void Send_UART_word(char* word)
{
    while(*word) {
        Send_UART(*word++);
//        word ++; // Lay ky tu tiep theo
    }
}

void Send_UART_numb(float number) {
    char data[8];
    ftoa(number, data,2);
    Send_UART_word(data);
}

void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}


void ftoa(float n, char *res, int afterpoint)
{
    float tmp = n;
        if (n<0) n = fabs(n);
        // Extract integer part
        int ipart = (int)n;

        // Extract floating part
        float fpart = n - (float)ipart;

        // convert integer part to string
        int i = intToStr(ipart, res, 0);

        // check for display option after point
        if (afterpoint != 0)
        {
            res[i] = '.';  // add dot

            // Get the value of fraction part upto given no.
            // of points after dot. The third parameter is needed
            // to handle cases like 233.007
            fpart = fpart * pow(10, afterpoint);

            intToStr((int)fpart, res + i + 1, afterpoint);
        }
        if(tmp<0)
        {
            int i = 0;
            for(;res[i] != '\0'; i++);
            reverse(res, i);
            res[i] = '-';
            res[i+1] = '\0';
            reverse(res,i+1);
        }
}
void Config_IO() {
    P1DIR = P2DIR = 0xFF;
    P1OUT = P2OUT = 0xFF;

    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;
}






