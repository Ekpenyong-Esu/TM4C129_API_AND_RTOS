/**
 * main.c
 */
#include <stdio.h>
#include <stdint.h>
#include <tm4c129encpdt.h>


#define GPIO_PIN_0              0x00000001  // GPIO pin 0
#define GPIO_PIN_1              0x00000002  // GPIO pin 1
#define GPIO_PIN_2              0x00000004  // GPIO pin 2
#define GPIO_PIN_3              0x00000008  // GPIO pin 3
#define GPIO_PIN_4              0x00000010  // GPIO pin 4

int main ()
{
    //uint32_t value;

    // setting up the clock of gpio port n and f
    SYSCTL_RCGCGPIO_R |= (SYSCTL_RCGCGPIO_R12|SYSCTL_RCGCGPIO_R5);


    // allow the clock of port n and f to stabillize
    while(!( SYSCTL_RCGCGPIO_R & (SYSCTL_RCGCGPIO_R12|SYSCTL_RCGCGPIO_R5))){};


    // setting up the clock of gpio portj
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;

    // the pj clock to stabilize
    while (!(SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R8)){};


    // enabling the gpio port n as output by setting the gpiodir to 1
    GPIO_PORTN_DIR_R |= (GPIO_PIN_1|GPIO_PIN_0);

    // enable the digital enable of portn so tha we can use it digital function
    GPIO_PORTN_DEN_R |= (GPIO_PIN_1|GPIO_PIN_0);


    // portF block confiquration
    GPIO_PORTF_AHB_DIR_R |= (GPIO_PIN_4|GPIO_PIN_0);


    // enabling portf for digital input
    GPIO_PORTF_AHB_DEN_R |= (GPIO_PIN_4|GPIO_PIN_0);



    // setting the gpio of portj as input
    GPIO_PORTJ_AHB_DIR_R &= ~(GPIO_PIN_1|GPIO_PIN_0);


    // enabble the gpioden of portj
    GPIO_PORTJ_AHB_DEN_R = (GPIO_PIN_1|GPIO_PIN_0);


    // setting the alternate function sellect of pin pj0 and pj1 to 0;

    GPIO_PORTJ_AHB_AFSEL_R &= ~(GPIO_PIN_1|GPIO_PIN_0);

    // set the pull up resistors of pj0 and pj1 to high = 1

    GPIO_PORTJ_AHB_PUR_R |= (GPIO_PIN_1|GPIO_PIN_0);

    // entering the forever loop

    while(1){

        // if pj0 button is pressed

        if ((GPIO_PORTJ_AHB_DATA_R & GPIO_PIN_0 )==0x0){

            // lit the D1 = gpio portn 1
            GPIO_PORTN_DATA_R |= (GPIO_PIN_1|GPIO_PIN_0);
        }
        else {

            // off the led 1
            GPIO_PORTN_DATA_R &= ~(GPIO_PIN_1|GPIO_PIN_0);
        }

        // pj1 is pressed
        if (((GPIO_PORTJ_AHB_DATA_R) & (GPIO_PIN_1)) !=GPIO_PIN_1){


            // lit led 2, which is PN0
            GPIO_PORTF_AHB_DATA_R |= (GPIO_PIN_4|GPIO_PIN_0);

        }
        else{

            // off led 2
            GPIO_PORTF_AHB_DATA_R &= ~(GPIO_PIN_4|GPIO_PIN_0);
        }
    }

}
