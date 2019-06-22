#include <msp430.h> 
#include <stdint.h>

/**
 * main.c
 */
#define FLASH_MEM   0x1040
#define MAX_FLASH_SIZE  10


volatile int fCount = 0;
volatile int *flash_ptr ;
void configClocks(void);
void configFlash(void);
void clearFlash(void);
void writeFlash( uint16_t );

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	configClocks();
	configFlash();
	int j;
	for (j = 0; j < MAX_FLASH_SIZE; j++)
	    writeFlash(j);
	__no_operation();
	writeFlash(123);
	__no_operation();
	writeFlash(111);
	__no_operation();
	return 0;
}
void configFlash(void)
{
    flash_ptr = (int *) FLASH_MEM;
    clearFlash();
}
void clearFlash(void)
{
    FCTL2 = FWKEY + FSSEL_2 + FN0;
    FCTL3 = FWKEY;
    FCTL1 = FWKEY+ ERASE;
    int i ;
    for (i = 0 ; i < MAX_FLASH_SIZE ; i ++)
        *(flash_ptr+i) = 0;
    FCTL3 = FWKEY + LOCK;
}
void configClocks(void)
{
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
}


void writeFlash( uint16_t num)
{
    FCTL2 = FWKEY + FSSEL_2 + FN0;
    FCTL3 = FWKEY;
    FCTL1 = FWKEY+ WRT;


    if (fCount < MAX_FLASH_SIZE)
    {
        *(flash_ptr+fCount) = num;
        fCount ++;
    }
    else
    {
        clearFlash();
        fCount = 0;
        writeFlash(num);
    }
    FCTL3 = FWKEY + LOCK;
}
