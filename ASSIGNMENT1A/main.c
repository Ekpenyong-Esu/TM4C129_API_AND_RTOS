#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "drivers/pinout.h"
#include "drivers/buttons.h"

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
    unsigned char ucDelta, ucState;

    // configure the device pins
    PinoutSet(false, false);

    // Initialise the button driver
    ButtonsInit();

    // Enable the GPIO pin for the LED (PN0).
    // Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    // loop forever
    while(1){
        // Pool the buttons.
        ucState = ButtonsPoll(&ucDelta,0);

        // The LED must be turned off first.
        LEDWrite(CLP_D1,0);

        if(BUTTON_PRESSED(RIGHT_BUTTON, ucState, ucDelta))

            while(1){
                // Turn on D1.
                 LEDWrite(CLP_D1,1);

                // Delay the LED
                //for(ucState = 0; ucState < 400000; ucState++) {}
                 SysCtlDelay(4000000);

                // Here we turn off LED
                 LEDWrite(CLP_D1,0);

                // We delay the LED again
                //for (ucState = 0; ucState < 400000; ++ucState) {}
                 SysCtlDelay(400000);
           }
      }
}

