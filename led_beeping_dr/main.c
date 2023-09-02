#include <stdio.h>
#include <stdint.h>
#include "tm4c129encpdt.h"

#define delay_value   200000


int main (void)
{
    volatile unsigned long ulLoop;

    // Enable clock for gpio port N using the clock gatting register.

      SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12 ;

    while(!(SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R12)){};

    //Enabling system clock gatting for portf.

     SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R5;

    // Waiting for the portf to be ready.

    while(!(SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R5)){};

    // Set portN led pins  as output pins

    GPIO_PORTN_DIR_R = (3<<0);

    // Enabling portf led pins as ouput.

    GPIO_PORTF_AHB_DIR_R  = (17<<0);




    // Enabling digital function for portn led pins.

    GPIO_PORTN_DEN_R = (3<<0);


    // Enabling digital function for portf led pins.


    GPIO_PORTF_AHB_DEN_R = 0x11;



    // Forever loop

    while(1)
    {

        GPIO_PORTN_DATA_R = (3<<0);

        for(ulLoop=0; ulLoop<delay_value; ulLoop++){}

        GPIO_PORTN_DATA_R = ~(3<<0);

        for(ulLoop=0; ulLoop<delay_value; ulLoop++){}

        GPIO_PORTF_AHB_DATA_R = (17<<0);

        for(ulLoop=0; ulLoop<delay_value; ulLoop++){}

        GPIO_PORTF_AHB_DATA_R = ~(17<<0);

        for(ulLoop=0; ulLoop<delay_value; ulLoop++){}

    }



}
