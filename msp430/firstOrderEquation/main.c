#include <msp430.h> 
#include <stdint.h>
#include <math.h>
/**
 * main.c
 */
void Config_IO();
void ConfigClock(void);
void ConfigUART(void);
void Send_UART(uint8_t);
void Send_UART_word(char*);
void Send_UART_numb(float);

void ftoa(float, char*,uint16_t);
//int intToStr(uint16_t, char*, uint16_t);
int IntToStr(char *out, int numb, int max_size);
//void reverse(char*, uint8_t);
void Send_UART_numb_int(int numb);

float GetNumb(void);

//
int IntToStr(char *out, int numb, int max_size)
{
//    printf("Process: %d with max_size %d\n", numb, max_size);
    int res = 0;
    if(numb < 0){
        out[res++]  = '-';
        max_size--;
        numb = -numb;
    }
    if(numb < 10){
        out[res++] = numb + '0';
    }else{
        res += IntToStr(&out[res], numb/10, max_size - 1);
        out[res++] = numb % 10 + '0';
    }
//    out[res++]='\0';
    return res;
}

int DblToStr(char *out, double numb, int max_size)
{
    int iNumb = (int)numb;
    int res = IntToStr(out, iNumb, max_size);
    numb -= iNumb;
    max_size -= res;
    double min_value =  1.0/pow(10, max_size);
    if(numb > min_value){
        out[res++] = '.';
    }

    while(numb > min_value && res < max_size){
        numb = numb *10;
        iNumb = numb;
        out[res++] = iNumb + '0';
        numb -= iNumb;
    }
    out[res++]='\0';
    return res;
}
    //

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    ConfigClock();
    Config_IO();
    ConfigUART();
    while(1)
    {
    float a,b,c,delta,x1,x2;
        Send_UART_word("Phuong trinh bac 2 co dang ax^2 + bx + c = 0. Nhap a :");
        a = GetNumb();
        Send_UART_word("Nhap b :");
        b = GetNumb();
        Send_UART_word("Nhap c :");
        c = GetNumb();
        if (a==0)
        {
            if (b==0)
            {
                if (c==0) Send_UART_word("Phuong trinh vo so nghiem");
                else Send_UART_word("Phuong trinh vo nghiem");
            }
            else
            {
                x1=-b/c;
                Send_UART_word("Phuong trinh co 1 nghiem:");
                Send_UART_numb(x1);
            }
        }
        else
        {
            delta=b*b-4*a*c ;
            if (delta<0) Send_UART_word("Phuong trinh vo nghiem");
            if (delta==0)
            {
                x1=-b/(2*a);
                Send_UART_word("Phuong trinh da nghiem kep: ");
                Send_UART_numb(x1);
            }
            if (delta>0)
            {
                x1=(-b+sqrt(delta))/(2*a);
                x2=(-b-sqrt(delta))/(2*a);
                Send_UART_word("Phuong trinh co 2 nghiem phan biet");
                Send_UART_numb(x1);
                Send_UART('\n');
                Send_UART_numb(x2);
                Send_UART('\n');
            }
        }
    }


   // Send_UART_numb(x);

	return 0;
}

float GetNumb(void)
{
    int iNumb= 0;
    int fNumb = 0;
    int pos = 1;

//            uint8_t buffer[10] = {'\0'};
            uint8_t ch = 0;
//            int index = 0;

    uint8_t decimal = 0;
            while(ch != '\n'){
                while(!(IFG2 & UCA0RXIFG));
                ch = UCA0RXBUF;
//                Send_UART_word("Ban da nhap vao: ");
//                Send_UART_numb_int(ch);
                Send_UART(ch);
                if(ch == '-'){
                    pos = -pos;
                }
                if(ch == '.'){
                    decimal = 1;
                }
                if(ch > 47 && ch < 58)
                if(decimal){
                    fNumb += fNumb *10 + ch - '0';
                    decimal++;
                }else{
                    iNumb = iNumb* 10 + ch - '0';
                }
//                buffer[index++] = ch;
            }
//            buffer[index] = 0;
//            tmp = atof(buffer);
            float res = pos*(iNumb + (float)fNumb/pow(10, decimal? decimal - 1:decimal));
    Send_UART_word("Your number: ");
    Send_UART_numb(res);
    Send_UART('\n');
    return res;
}
void ConfigClock(void)
{
    DCOCTL = CALDCO_8MHZ;
    BCSCTL1 = CALBC1_8MHZ;

    BCSCTL1 |= XT2OFF;
    BCSCTL2 |= SELM_1 + DIVS_3;
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
    IE2 |= UCA0RXIE;

}

void Send_UART(uint8_t data) {
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
    char data[10];
    DblToStr(data,number,10);

    Send_UART_word(data);
}

void Send_UART_numb_int(int numb)
{
    char a[10];
    uint8_t res = IntToStr(a, numb, 10);
    a[res++] = '\0';
    Send_UART_word(a);
}
void Config_IO() {
    P1DIR = P2DIR = 0xFF;
    P1OUT = P2OUT = 0xFF;

    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;
}


//#pragma vector=USCIAB0RX_VECTOR
//__interrupt void USCI0RX_ISR(void) {
//    unsigned char newData = UCA0RXBUF;
//    if(newData=='d') {
//        P1OUT ^= BIT0;
//        P2OUT ^= BIT0;
//    }
//    Send_UART(newData);
//}
//void reverse(char *str, uint16_t len)
//{
//    int i=0, j=len-1, temp;
//    while (i<j)
//    {
//        temp = str[i];
//        str[i] = str[j];
//        str[j] = temp;
//        i++; j--;
//    }
//}

//int intToStr(uint16_t x, char str[], uint16_t d)
//{
//    int i = 0;
//    while (x)
//    {
//        str[i++] = (x%10) + '0';
//        x = x/10;
//    }
//
//    // If number of digits required is more, then
//    // add 0s at the beginning
//    while (i < d)
//        str[i++] = '0';
//
//    reverse(str, i);
//    str[i] = '\0';
//    return i;
//}


//void ftoa(float n, char *res, uint16_t afterpoint)
//{
//    float tmp = n;
//            if (n<0) n = fabs(n);
//            // Extract integer part
//            int ipart = (int)n;
//
//            // Extract floating part
//            float fpart = n - (float)ipart;
//
//            // convert integer part to string
//            int i = s(ipart, res, 0);
//
//            // check for display option after point
//            if (afterpoint != 0)
//            {
//                res[i] = '.';  // add dot
//
//                // Get the value of fraction part upto given no.
//                // of points after dot. The third parameter is needed
//                // to handle cases like 233.007
//                fpart = fpart * pow(10, afterpoint);
//
//                intToStr((int)fpart, res + i + 1, afterpoint);
//            }
//            if(tmp<0)
//            {
//                int i = 0;
//                for(;res[i] != '\0'; i++);
//                reverse(res, i);
//                res[i] = '-';
//                res[i+1] = '\0';
//                reverse(res,i+1);
//            }
//}
