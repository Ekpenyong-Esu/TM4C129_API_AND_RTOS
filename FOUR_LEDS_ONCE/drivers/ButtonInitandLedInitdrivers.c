/*
 * ButtonInitandLedInitdrivers.c
 *
 *  Created on: 25 Nov 2021
 *      Author: MAHONRI
 */

#include <drivers/ButtonInitandLedInitdrivers.h>

void Led_pin(){


    // Enable clock for gpio port N using the clock gatting register.

    SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R12 ;

    while(!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R12)){};


    SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R5 ;

    while(!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)){};



    // Set portN pin 1 AND O as output pin

    GPIO_PORTN_DIR_R = (GPIO_PIN_1 |GPIO_PIN_0);

    // Enabling digital function.

    GPIO_PORTN_DEN_R = (GPIO_PIN_1 |GPIO_PIN_0);



    // Set portF pin 4 AND O as output pin

    GPIO_PORTF_AHB_DIR_R  = (GPIO_PIN_4|GPIO_PIN_0);


    // Enabling digital function.

    GPIO_PORTF_AHB_DEN_R = (GPIO_PIN_4|GPIO_PIN_0);

}

void buttons_init(){

    // setting up the clock of gpio portj
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;

    while(!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R8)){};


    // enabble the gpioden of portj
    GPIO_PORTJ_AHB_DEN_R = (GPIO_PIN_1|GPIO_PIN_0);


    // setting the alternate function sellect of pin pj0 and pj1 to 0;

    GPIO_PORTJ_AHB_AFSEL_R &= ~(GPIO_PIN_1|GPIO_PIN_0);

    // set the pull up resistors of pj0 and pj1 to high = 1

    GPIO_PORTJ_AHB_PUR_R |= (GPIO_PIN_1|GPIO_PIN_0);

    // entering the forever loop

}

void delayMs(int n){
    int i,j;
    for(i=0; i<n; i++)
        for(j=0; j<3180; j++)
        {}                         // do nothing for 1Ms
}
