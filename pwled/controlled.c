//*****************************************************************************
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/tm4c129encpdt.h"



//***********************************************************************
//                       Configurations
//***********************************************************************
// Configure the UART.
void ConfigureUART(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);



}

//*****************************************************************************
//                      Main
//*****************************************************************************
int main(void)
{
    ConfigureUART();

    float pwm_word;
    uint32_t systemClock;
    systemClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 16000);

    static char buffer[8];
    long val;
    int on = 0;

    pwm_word = systemClock / 200;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinConfigure(GPIO_PF2_M0PWM2);

    // Setting the PWM synchronisation
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DBG_RUN);

    // Setting the period pwm_word
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, pwm_word);

    // Enbling the counter for PWM_!
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);


    while(1)
        {

        //Waiting and getting input from user.
        UARTgets(buffer,sizeof(buffer));


        //Displaying the User input by
        UARTprintf("User wrote string: '%s'\n",buffer);


        //Using strtol() function to convert character input to long
        val = strtol(buffer, NULL, 10);


        //Setting the special condition from the Assignment from 0% to 100%
        //Hence, if the input is less than zero OR greater than 100 print INVALID INPUT.
        if(val < 0 || val > 100 &&  on ==1){


            // Printing out the users input
            UARTprintf("invalid input \n");

        }else{

            // Else if the user input is within the range of 0 to 100 Set pwm_word get the pwm output
            // hence the the width is pwm_word * (0.01 * val)
            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, pwm_word*(0.01*val));
        }

        //Setting PWM special case in a situation where users input = o
        if (val == 0){


            // If the user input = 0, PWM_OUT_2_BIT = 0 ( that is LOW)
            PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);

            // But if users input = 100 the PWM brightens the LED
        }else if(val == 100){


            // Setting the State as TRUE or 1
            PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);


        }

    }
}

