#include "UART.h"


/**
 * main.c
 */

#define MAX_INPUT_SIZE        3
#define SLEEP           -2
#define QUES            -1
#define INPUT_NONE      0
#define INPUT_RECORDING 1
#define INPUT_READY     2
#define INPUT_DONE      3
#define INPUT_RESET     4
//#define INPUT_EMPTY     3
#define MAX_RX_SIZE     10
//void process(void);
//int process_number();
void send_uart(uint8_t *tx_p, uint8_t size);

void sysReset(void);
void numbReset(void);
//volatile uint16_t count = 0;
volatile uint8_t decimal;
volatile int pos;


volatile float input[MAX_INPUT_SIZE] ;


volatile uint8_t state;
volatile uint8_t rx_size = 0;
volatile uint8_t rx_buf[MAX_RX_SIZE];
volatile uint8_t tx_size;
volatile uint8_t tx_count;
volatile uint8_t *tx_buf;
void numbProcess();
void intro(void);
void ques(void);
void solve(void);
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    ConfigClock();
    //Config_IO();
    P1DIR |= BIT0;
    ConfigUART();
    __bis_SR_register(GIE);
    state = 1;
    decimal = 0;
    pos = 1;
    intro();
    while (1)
    {

        if(!state){
            solve();
            intro();
            state = 1;
        }


        P1OUT |= 0x01;              // toggle P1.0
        _delay_cycles(1000000);
        P1OUT &= 0x00;
        _delay_cycles(1000000);

    }
    return 0;
}

void intro(void)
{
    Send_UART_word("Please enter your input :\na = ");
    input[0] = input[1] = input[2] = 0;
    decimal = 0;
    pos = 1;
//    state = QUES;
}
//void ques(void)
//{
//    Send_UART('\n');
//    Send_UART(inputName[count]);Send_UART_word(" = ");
////    state = SLEEP;
//}
void sysReset(void)
{
//    count = 0;
    int i;
    for (i = 0; i < MAX_INPUT_SIZE; i++)
    {
        input[i] = 0;
    }
}

void solve(void)
{

#define a input[0]
#define b  input[1]
#define c  input[2]
    float x1, x2, delta;
    if (a==0)
    {
        if (b==0)
        {
            if (c==0) Send_UART_word("Phuong trinh vo so nghiem");
            else Send_UART_word("Phuong trinh vo nghiem");
        }
        else
        {
             x1=-c/b;
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
void send_uart(uint8_t *tx_p, uint8_t size)
{
    while(tx_size){
        // TODO: Go to sleep

    }
    tx_buf = tx_p;
    tx_size = size;
    IE2 |= UCA0TXIE;
}

void send_uart_from_isr(uint8_t tx_p){
    while(tx_count < tx_size){
        while(!(IFG2 & UCA0TXIFG));
        UCA0TXBUF = *(tx_buf +  tx_count++);
    }
    UCA0TXBUF = tx_p;
    tx_size = 0;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    unsigned char ch = UCA0RXBUF;
    send_uart_from_isr(ch);
//    if(state != INPUT_NONE){
//        // TODO: print warning ! error
//        return;
//    }

    if(ch == '\n'  ){
        input[state - 1] = pos > 0? input[state - 1]:-input[state - 1];
        state++;
        if(state > MAX_INPUT_SIZE){
            state = 0;
        }else{
            send_uart_from_isr(state + 'a' - 1);
            send_uart(" = ", 3);
        }
        decimal = 0;
        pos = 1;
    }else if( ch == '\r')
    {
//        send_uart_from_isr('e');
    }
    else if(ch == '.'){
        decimal = 1;
    }else if(ch == '-'){
        pos = -pos;
    }else if(ch>='0' && ch <= '9' && state){

        if(decimal)
       {
            input[state - 1] = input[state - 1] + (float)(ch - '0')/pow(10, decimal);
                   decimal ++;
       }
       else
       {
           input[state - 1] = input[state - 1] * 10 + (float)(ch - '0');
       }
    }else{
        // TODO: reset || warning!!!
        return;
    }
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void){

    if(tx_count < tx_size){
        UCA0TXBUF = *(tx_buf +  tx_count++);
    }else{
        tx_count = 0;
        tx_size = 0;
        IE2 &= ~UCA0TXIE;
        // if sleep, wake up!
    }
}
