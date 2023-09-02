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
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
/* Peripheral Configuration Statement
* Set the type of the GPIO Pin */
GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
/* GPIO Pin 1 on PORT F initialized to 0, RED LED is off */
GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1,0);
/* Application Loop */
while(1)
{
/* Make Pin High */
GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
/* Delay for 100ms */
SysCtlDelay(1000000);
/* Make Pin Low */
GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1,0);
/* Delay for 100ms */
SysCtlDelay(1000000);
}
}
