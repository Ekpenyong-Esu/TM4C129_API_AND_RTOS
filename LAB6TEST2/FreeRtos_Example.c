#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/buttons.h"
#include "drivers/pinout.h"
#include "FreeRTOS.h"
#include "driverlib/pin_map.h"
#include "task.h"
#include "semphr.h"
#include "driverlib/timer.h"
#include "inc/tm4c129encpdt.h"
#include "driverlib/uart.h"

uint32_t system_clock;
uint32_t g_ui32Flags;

char output[16];
int test = 0;
int status = 0;


void ConfigureUART(uint32_t system_clock, uint32_t baud_rate)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    UARTConfigSetExpClk(UART0_BASE, system_clock, baud_rate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

void UARTStringPut(char *string)
{

    // Send the string.
    while(*string)
    {
        UARTCharPut(UART0_BASE,*string++);
    }

}

// Will update the string displayed on the terminal.
void UpdateDisplay(char* print)
{

    UARTStringPut( "\e[1;1H\e[2J" );

    UARTCharPut(UART0_BASE, '\r');

    UARTCharPut(UART0_BASE, '\r');

    while(*(print) != '\0')
    {
        UARTCharPut(UART0_BASE, *print++);
    }


    if(status==1){

        char counterString[15];

        UARTCharPut(UART0_BASE, '\n');
        UARTCharPut(UART0_BASE, '\r');

        sprintf(counterString, "%d", test);

        int i = 0;
        while(counterString[i] != '\0')
        {
            UARTCharPut(UART0_BASE, counterString[i]);
            i++;
        }
    }
}

void shift(char input){

    int i;

    for(i = 0; i < sizeof(output) - 2; i++){
        output[i] = output[i+1];

    }

    output[i] = input;

}

void vTaskGetInput(void* pvInput){


    while(1){

        char input = UARTCharGet(UART0_BASE);

        shift(input);
        test++;

        UpdateDisplay(output);
    }


}

void vTaskButton(void* pvInput){

    unsigned char ucDelta, ucState;

    while(1){

        ucState = ButtonsPoll(&ucDelta, 0);  //Poll buttons

        if(BUTTON_PRESSED(LEFT_BUTTON, ucState, ucDelta)){

           status = 1;

           UpdateDisplay(output);
           vTaskDelay(10000);

           status = 0;
           UpdateDisplay(output);
        }

        vTaskDelay(25);  //wait a short delay before polling again
    }
}


void main(void)
{

    xTaskHandle xTaskInput, xTaskButton;

    char inputString[27] = "Embedded systems is so fun";
    int i;

    uint32_t systemClock;
    systemClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 16000);
    ButtonsInit();

    ConfigureUART(systemClock, 115200);    // configure and initialize UART

    output[15] = '\0';

    for(i = 0; i < sizeof(output)-1; i++){
        output[i] = ' ';
    }

    for(i = 0; i < sizeof(inputString)-1; i++){
        shift(inputString[i]);
        test++;
    }

    UpdateDisplay(output);

    xTaskCreate(vTaskGetInput, "Gets character input from UART", configMINIMAL_STACK_SIZE, (void*) 0, tskIDLE_PRIORITY + 1, &xTaskInput);
    xTaskCreate(vTaskButton, "Controls the button", configMINIMAL_STACK_SIZE, (void*) 0, tskIDLE_PRIORITY + 2, &xTaskButton);

    vTaskStartScheduler();


}

