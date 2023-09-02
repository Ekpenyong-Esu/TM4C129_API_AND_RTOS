



/**
 * main.c
 */
#include <stdio.h>
#include <stdint.h>
#include "tm4c129encpdt.h"

#define DELAY_VALUE  200000


int main(void)
{
    volatile unsigned long ulLoop;

    // setting the clock for gpio portA

    SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R0;

    // allow the portA clock to settle
    while(!(SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R0)){};

    // setting the direction of the gpio portA as output.

    GPIO_PORTA_AHB_DIR_R |= (0xFF<<0);

    // setting the digital output of gpio porta

    GPIO_PORTA_AHB_DEN_R |= (0XFF<<0);


    while(1){

        // writting to the gpiodata register of portA
        GPIO_PORTA_AHB_DATA_R |=0XFF;

        for(ulLoop=0; ulLoop < DELAY_VALUE; ++ulLoop){}

        GPIO_PORTA_AHB_DATA_R |= ~0XFF;

        for(ulLoop=0; ulLoop < DELAY_VALUE; ++ulLoop){}

    }



}
