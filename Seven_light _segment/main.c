#include <stdio.h>
#include <stdint.h>
#include <tm4c129encpdt.h>

/* we are displaying only number 14 ( 1 and four in the seven segment)*/


#define SYSTEM_CLOCK_FEQ        25000000
#define DELAY_VALUE             SYSTEM_CLOCK_FEQ/25
#define DELAY_REBOUNCE          SYSTEM_CLOCK_FEQ/1000

int main  (){
    volatile unsigned uLoop = 200000;
    unsigned int      i;

unsigned char Seven_seg_pattern[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

    // enable portA and portB clock so that we can make use of it
    SYSCTL_RCGCGPIO_R | (SYSCTL_RCGCGPIO_R1|SYSCTL_RCGCGPIO_R0);

    // allow the clock to settle
   while (!(SYSCTL_RCGCGPIO_R & (SYSCTL_RCGCGPIO_R1|SYSCTL_RCGCGPIO_R0))){};


   // we sett port b pins as output from 0 to 7 so that we can use it for the segment
   GPIO_PORTB_AHB_DIR_R |= (0xFF);  // you can also use (255<<0) left shift and start from zero

   // enable the digital enable register so that we can write to it
   GPIO_PORTB_AHB_DEN_R |= (255<<0);

   // enable port a as output so that we can use it to sellect the output and write the digit
   GPIO_PORTA_AHB_DIR_R |= (0XC0);// we are only activating th 7 and 6 bit of port a since
                                  // since need to display only 1 and 4 we can also use 192 since 192 is the decimal
                                  // value of 0xC0 (0xC0 is hexadecimal value)

   // enable portA digital so that we can write to it using data reg
   GPIO_PORTA_AHB_DEN_R |= (192<<0);


   while (1){

       // we first drive the pattern of 1 in the segment first
       GPIO_PORTB_AHB_DATA_R |= Seven_seg_pattern[1];

       // we use port A to sellect the left segment
       GPIO_PORTA_AHB_DATA_R |= (0x80); // we can also use (128) since it is the decimal value of 0x80

       // delay for 20Ms
       for( i=0; i<uLoop; ++i){};


       // we now drive the pattern of 4
       GPIO_PORTB_AHB_DATA_R |= Seven_seg_pattern[4];

       // we use port A to sellect the didgit for the parttern
       GPIO_PORTA_AHB_DATA_R |= (0x40);    // we can also use (64) since 64 is the decimal value of 0x40.

       // we now delay for 20Ms
         for( i=0; i<uLoop; ++i){};

   }



}
