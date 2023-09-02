#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "drivers/buttons.h"
#include "drivers/pinout.h"

// The error routine that is called if the driver library
// encounters an error
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

// Main Function
int main(void)
{


    // configure the device pins
    PinoutSet(false, false);

    // Initialise the button driver
    ButtonsInit();

    // Enable the GPIO pin for the LED (PN0), (PN1), (PF4),(PF0).
    // Set the direction as output, and
    // enable the GPIO pins for digital functions.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);


    // loop forever
    while(1){



        // Set LED D1 high
        //LEDWrite(CLP_D1,1);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);

        // Delay the LED for 1000000
        SysCtlDelay(1000000);

        // Set LED D1 low
        //LEDWrite(CLP_D1,0);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);


        //Set LED D2 high

        // LEDWrite(CLP_D2,1);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);

        // Delay for 1000000
        SysCtlDelay(1000000);

        // Set the LED D2 low
        //LEDWrite(CLP_D2,0);GPIO_PIN_1
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);


        // Set the LED D3 high
        //LEDWrite(CLP_D3,1);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);

        //  Delay for 1000000
        SysCtlDelay(1000000);


        // Set the LED D3 Low
        //LEDWrite(CLP_D3,0);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);


        // Set LED D4 High
        //LEDWrite(CLP_D4,1);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 1);

        // Delay for 1000000
        SysCtlDelay(1000000);

        // Set LED D4 Low
        //LEDWrite(CLP_D4,0);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0);



        // reverse LED D3 To High
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);

        // Delay the LED D3
        SysCtlDelay(1000000);

        // Reverse LED LOW
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);



        // reverse LED D2 High
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0 );


        // Delay the LED
        SysCtlDelay(1000000);

        // Reverse the LED  D2 Low
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);







    }

}
