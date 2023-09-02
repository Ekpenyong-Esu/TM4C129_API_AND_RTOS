#include <stdio.h>
#include <stdint.h>
#include <tm4c129encpdt.h>

#define GPIO_PIN_0              0x00000001
#define GPIO_PIN_1              0x00000002

int main (){
    uint32_t count = 0;

    //WE FIRST ACTIVATE THE SYSTEM CLOCK

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;

    // ALLOW THE CLOCK TO SETTLE DOWN

    while(!(SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12)){};


    // ACTIVATING THE PORTM PIN 1 FOR OUTPUT
    GPIO_PORTN_DIR_R |= GPIO_PIN_0|GPIO_PIN_1;


    // ENABLING DIGITAL ENABLE
    GPIO_PORTN_DEN_R |= GPIO_PIN_0|GPIO_PIN_1;

    //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R& ~(15<<4));



    // WE NOW CONFIGURE THE SYSTEM TICKS
    // WE FIRST LOAD THE SYSTICK REGISTER WHICH IS NVIC_ST_RELOAD_R

    NVIC_ST_RELOAD_R |= 12500000-1;

    // WE NOW ACTIVATE THE ENABLE PIN AND SET THE CLOCK SOURCE TO ONE (IN ORDER TO USE SYSTEM CLOCK)

    NVIC_ST_CTRL_R |= 5;


    // WE INITIATE FOREVER LOOP

    while (1){

        if (NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT ){

            count++;


            // IF COUNT OF STCTRL FLAG IS SET


            GPIO_PORTN_DATA_R = count;
        }

    }



}
