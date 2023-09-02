//*****************************************************************************
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <driverlib/rom.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/tm4c129encpdt.h"



#include "driverlib/adc.h"





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

//PWM MAPPING
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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

    // Declaing my PWMvalue as an integer with four bytes
    int pwmValue;

    pwm_word = systemClock / 200;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinConfigure(GPIO_PF2_M0PWM2);

    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DBG_RUN);

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, pwm_word);

    PWMGenEnable(PWM0_BASE, PWM_GEN_1);


    uint32_t ADCvalue[1];


    //WE Enable the System ANALOGUE TO DIGITAL PERIPHERAL ADC0 for input
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);


    // Setting up the ONE SEQUENCE TO 3
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE | ADC_CTL_END);


    // Enabling the processor  SEQUENCE TRIGGER
    ADCSequenceEnable(ADC0_BASE, 3);


    // Clearing the SAMPLE SEQUENCE INTERRUPT SOURCE and wait for the write buffer
    //
    ADCIntClear(ADC0_BASE, 3);

    while(1)
    {

        // This cause the processor trigger sequence to read
        ADCProcessorTrigger(ADC0_BASE, 3);


        // Waiting for conversion
        while(!ADCIntStatus(ADC0_BASE, 3, false))
        {
        }


        // We clear the Sample interrupt source and wait for the buffer
        ADCIntClear(ADC0_BASE, 3);


        // Getting the capture sequence data and read
        ADCSequenceDataGet(ADC0_BASE, 3, ADCvalue);


        //Map the sequence from  ~2000 to ~4000 to 0 to 100 (%) and assigning the value To pwmValue
        pwmValue = map(ADCvalue[0], 2060, 4080, 0, 100);


        //Turn of the LED if PWM less than or equall to zero, and set the pwmValue to 1
        if(pwmValue <= 0){
            PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
            pwmValue = 1;


        }else{

            // Else set the PWM_OUT_2_BIT to TRUE
            PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);

        }

        // SEtting a condition if is IMPOSSible to get a pwmValue of 100, we find the nearest value approximation
        if (pwmValue >= 97){
            pwmValue = 100;

        }

        //GEtting the output of the PWM
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, pwm_word*0.01*pwmValue);



        //The print the OUtPut to the DESktop
        UARTprintf("Pwm value: %i \r", pwmValue);

    }
}

