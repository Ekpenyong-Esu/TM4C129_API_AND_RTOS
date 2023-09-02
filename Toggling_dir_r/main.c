#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <driverlib/gpio.h>
#include <inc/tm4c129encpdt.h>


#define delay_value   200000

int main(void)
{
    volatile unsigned long ulLoop;

    // Enable clock for gpio port N using the clock gatting register.

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;

    while (!(SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R12))
    {
    };

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;

    while (!(SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R5))
    {
    };

    // Set portN pin 1 AND O as output pin

    GPIO_PORTN_DIR_R |= GPIO_PIN_0  | GPIO_PIN_1;

    // Set portF pin 4 AND O as output pin

    GPIO_PORTF_AHB_DIR_R = (17 << 0);

    // Enabling digital function.

    GPIO_PORTN_DEN_R = 0X02 | 0x01;

    // Enabling digital function.

    GPIO_PORTF_AHB_DEN_R = (17 << 0);

    GPIO_PORTF_AHB_DATA_R = (17 << 0);

    // Forever loop

    while (1)
    {

        GPIO_PORTN_DATA_R = (1 << 1);

        for (ulLoop = 0; ulLoop < delay_value; ulLoop++)
        {
        }

        GPIO_PORTN_DATA_R = ~0x02;

        for (ulLoop = 0; ulLoop < delay_value; ulLoop++)
        {
        }

        GPIO_PORTN_DATA_R = 0x01;

        for (ulLoop = 0; ulLoop < delay_value; ulLoop++)
        {
        }

        GPIO_PORTN_DATA_R = ~0x01;

        for (ulLoop = 0; ulLoop < delay_value; ulLoop++)
        {
        }

    }

}
