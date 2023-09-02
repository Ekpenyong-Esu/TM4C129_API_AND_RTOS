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
    //unsigned char ucDelta, ucState;
    unsigned char buttonOne, buttonTwo;

    // configure the device pins
    PinoutSet(false, false);

    // Initialise the button driver
    ButtonsInit();

    // Enable the GPIO pin for the LED (PN0).
    // Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    // LED should be 'OFF' in the beginning.
    LEDWrite(CLP_D1, 0);

    // loop forever
    while (1)
    {
        // Pool the buttons.
        buttonOne = ButtonsPoll(&buttonTwo, 0);

        //We toggle with the left button
        if (BUTTON_PRESSED(LEFT_BUTTON, buttonTwo, buttonOne))
        {

            // Reading the State of the pin
            if (GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_1) == 0)
            {
                LEDWrite(CLP_D1, 1);

            }

            else
            {
                LEDWrite(CLP_D1, 0);
            }
        }

        // If the right button is pressed and hold, 'ON' the LED.
        if (BUTTON_PRESSED(RIGHT_BUTTON, buttonOne, buttonTwo))
        {

            // ON the LED
            LEDWrite(CLP_D1, 1);
        }

        //Else If the right button is released 'OFF' The LED.
        else if (BUTTON_RELEASED(RIGHT_BUTTON, buttonOne, buttonTwo))
        {

            // 'OFF' the LED.
            LEDWrite(CLP_D1, 0);

        }

    }
}
