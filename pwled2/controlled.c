#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#define PWM_FREQUENCY 1000 // PWM frequency in Hz

void ConfigureUART(void) {
    // Enable UART0 and GPIOA peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure GPIO pins for UART0
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Initialize UART0
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);
}

int main(void) {
    // Initialize system clock to 16MHz
    uint32_t clockget = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 16000);

    // Initialize UART communication
    ConfigureUART();

    // Configure PWM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Peripheral, this mean enable portF
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);           // This mean to divide the pulse width clock by 1
    SysCtlPeripheralDisable(SYSCTL_PERIPH_PWM0);   // This mean to disable pulse width 0 of the MCU
    SysCtlPeripheralReset(SYSCTL_PERIPH_PWM0);      // Reset the pwm 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);     // Enable the PWM 0

    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);     // configure pin 2 of portf which is connected to the EDU led
    GPIOPinConfigure(GPIO_PF2_M0PWM2);               // attach the PWM to the pin and pork

    // Configure PWM generator
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // this allow for debugging the pwm
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, SysCtlClockGet() / PWM_FREQUENCY); // the period is set by dividing the clock freq by pwm freq 16000/1000

    // disable PWM output
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);   // Disable the Led before entering the while loop to decide the range of the value
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);                // Enable the pwm gen 1

    uint32_t pwmWidth = 0; // PWM pulse width (0 to 100%)

    while (1) {
        UARTprintf("Enter LED Brightness (0-100): ");
        char buffer[16];  // for holding the value taken from uart
        UARTgets(buffer, sizeof(buffer));

        // Convert input to an integer
        int input = atoi(buffer);

        // Ensure input is within valid range
        if (input >= 0 && input <= 100)
        {
            // Calculate PWM pulse width
            pwmWidth = (SysCtlClockGet() / PWM_FREQUENCY) * input / 100;
            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, pwmWidth);

            UARTprintf("LED Brightness set to %d%%\n", input);

            // Special case: Turn off LED
            if (input == 0) {
                PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, false);
            } else {
                PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
            }
        } else {
            UARTprintf("Invalid input. Please enter a value between 0 and 100.\n");
        }
    }
}
