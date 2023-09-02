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

    volatile uint32_t ui32Loop;

/* System Initialization Statements
 Set the clock to directly run from the crystal at 16MHz */
SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);

/* Peripheral Initialization Statement
* Set the clock for the GPIO Port F */
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);

while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ))
    {
    }

SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);


while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }


/* Peripheral Configuration Statement
* Set the type of the GPIO Pin, PF4 as input and PF1 as output */
GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0);
GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_1);

GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE,GPIO_PIN_1);
GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE,GPIO_PIN_0);

/* GPIO Pin 1 on PORT F initialized to 0, RED LED is off */
GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_1,0);
GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0,0);
/* Application Loop */
while(1)
{
/* Reading the state of input pin PF4
* If read state is logic low implies switch is pressed
* Turn on the LED at PF1 */
if(GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0)==0)
{
GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
}
/* If read state is logic high implies switch is not pressed
* Turn off the LED at PF1 */
else
{
GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
}



if(GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1)==0){
    while(1){

    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);

    SysCtlDelay(200000);

    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0,0);

    SysCtlDelay(200000);
}
}
}
}
