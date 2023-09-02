/* Defines boolean and integer data types */
#include <stdbool.h>
#include <stdint.h>

/* Defines the base address of the memories and peripherals */
#include "inc/hw_memmap.h"

/* Defines the common types and macros */
#include "inc/hw_types.h"

/* Defines and Macros for GPIO API */
#include "driverlib/gpio.h"

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"


int main(void)
{
    /* System Initialization Statements
     * Set the clock to directly run from the crystal at 16MHz */
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);

    /* Peripheral Initialization Statement
     * Set the clock for the GPIO Port F */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);


    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE,GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE,GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_0);

    /* Application Loop */
    while(1)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        SysCtlDelay(800000);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);

        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
        SysCtlDelay(800000);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);
        SysCtlDelay(800000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);

        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0, 1);
        SysCtlDelay(800000);
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0, 0);



    }
}
