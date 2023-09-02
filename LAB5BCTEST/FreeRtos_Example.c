/* CREATED BY EKPENYONG, EKPENYONG ESU*/

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/buttons.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"
#include "inc/tm4c129encpdt.h"
#include <stdio.h>
#include <USART.h>
#include "semphr.h"

// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

//FUNCTIONS DELARATIONS
void vTaskD2(void* pvParameters);
void vTaskD4(void* pvParameters);
void vTaskD1(void* pvParameters);

//DECLARING GLOBAL VARIABLE
xTaskHandle xHandle2;
SemaphoreHandle_t xSemaphore;

int missTime = 0;
char stringTime[10];


// Configuration of the LEDs
void configureLEDs(void){

    //SETIING UP THE DEVICE PINS IN ORDER TO USE IT AS GPIO OUTPUT
    PinoutSet(false, false);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // LED USE FOR DEBUGGING
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

    // USART_putString("\r\nLEDs configured: ");
}

//CONFIGURING UART
void configureUART(void){

    //ENABLING PORTA PINS USING FOR UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //WAIT FOR THE PERIPHERAL TO STABILIZE
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}

    // ENABLING THE UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //WAITING FOR THE UART0 TO STABILIZE
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}

    // CONFIGURING THE LEDS FOR DEBUGGING
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //SETTING THE BAUDRATE USING THE PRECISION INTERNAL OSCILLATOR
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    // CONFIGURING THE USART FOR 9600 BAUD RATE, NO PARITY BIT, AND 16MHz FREQUENCY
    UARTStdioConfig(0, 9600, 16000000);

    //USART_putString("\r\nUART configured ");
}

//USING THE LED FOR DEBUGGING
void vTaskD2(void *pvParameters) {

    //DECLARING LOCAL VARIABLE
    TickType_t xLastWakeTime;

    //Loop forever
    // while(1){// COMENTING THIS PART TO GET ONE COMPLETE CYCLE

    //CHECK IF THE SEMAPHORE CAN BE OBTAINED, IF NOT, WAIT FOR IT
    USART_putString("\r\nTask 1 has started: ");
    if(xSemaphoreTake(xSemaphore, portMAX_DELAY)){


        USART_putString("\r\nTask 1 has taken the semaphore:");

        //USING THE LEDS FOR DEBUGGING
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);

        //STAY ON FOR 4ms
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, 4000);

        // USING THE LED FOR DEBUGGING
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

        //RELEASE THE SEMAPHORE
        USART_putString("\r\nTask 1 has given the semaphore: ");
        xSemaphoreGive(xSemaphore);
        vTaskDelay(10000);
    }
    //}
}

//THIS TASK IS FOR TASK TWO
void vTaskD4(void *pvParameters) {

    //LOCAL VARIABLE DECLARATION
    TickType_t xLastWakeTime;
    const TickType_t releaseTask = 500;

    // Loop forever
    //  while(1){// COMENTING TO HAVE ONE COMPLETE CYCLE

    //Waits until release time
    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, releaseTask);

    // Turn on the LED D4
    // taskENTER_CRITICAL();
    USART_putString("\r\nTask 2 preempt task 1 and started without semaphore: ");
    USART_putString("\r\nTHERE IS INVERSION OF PRIORITY HERE: ");
    //taskEXIT_CRITICAL();
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);

    //STAYS ON FOR 2ms
    xLastWakeTime = xTaskGetTickCount();
    // vTaskDelayUntil(&xLastWakeTime, 2000);

    // USING THE LEDS FOR DEBUGGING
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
    taskENTER_CRITICAL();
    USART_putString("\r\nTask 2 ended: ");
    USART_putString("\r\nTask 1 ended: ");
    taskEXIT_CRITICAL();
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
    vTaskDelay(1000);

    // }// COMENTING THIS PART TO HAVE ONE COMPLETE CYCLE
}

//THIS TASK IS FOR TASK 3
void vTaskD1(void* pvParameters){

    // LOCAL VARAIABLE DECLARATION
    TickType_t xLastWakeTime, xEndTime, xExecTime;
    const TickType_t deadline = 1000;
    const TickType_t releaseTask = 500;

    // Loop forever
    // while(1){// COMENTING THIS TO SEE ONE COMPLETE CYCLE


    // CONFIGURING THE RELEASE TIME
    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, releaseTask);
    USART_putString("\r\nTask 3 preempt task 1 and started:");
    USART_putString("\r\nTask 3 is blocked by task 1 because of the semaphore:");
    USART_putString("\r\nTask 1 resumed:");

    //CHECK IF THE SEMAPHORE CAN BE OBTAINED, IF IT CAN'T, WAIT
    if(xSemaphoreTake(xSemaphore, portMAX_DELAY)){
        vTaskSuspend(xHandle2);
        USART_putString("\r\nTask 3 has taken the semaphore:");
        USART_putString("\r\nTask 3 resumed:");

        //TURNING THE LEDS ON
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);

        //STAY ON FOR 3ms
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, 3000);

        // LED IS USE FOR DEBUGGING
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

        // SEMAPHORE RELEASE BY TASK 3
        USART_putString("\r\nTask 3 has given the semaphore: ");
        xSemaphoreGive(xSemaphore);
        vTaskResume(xHandle2);

        //GETTING THE TASK END TIME TO FIND THE DIFFERENCE
        xEndTime = xTaskGetTickCount();
        xExecTime = (xEndTime-xLastWakeTime)*3000;

        if(xExecTime >= deadline){

            // CALCULATING THE MISS TIME
            missTime = xExecTime;
            sprintf(stringTime, "%d", missTime);

            USART_putString("\r\nTask 3 has missed the deadline by: ");
            USART_putString(stringTime);
            USART_putString(" TICKS");

        }
        else{
            USART_putString("\r\nTask 3 has succeeded: ");
        }

        USART_putString("\r\n1 completely finished cycle:");
        vTaskDelay(10000);
    }
    // }// COMENTING THIS TO SEE ONE COMPLETE CYCLES

}

void main(void){

    //INITIALIZING UART CONFIGURATIONS
    configureUART();
    configureLEDs();

    //CREATING SEMAPHORE
    vSemaphoreCreateBinary(xSemaphore);

    //Creation of the tasks
    xTaskCreate(vTaskD2, "LED D2", configMINIMAL_STACK_SIZE, & xSemaphore, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vTaskD4, "LED D4", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &xHandle2);
    xTaskCreate(vTaskD1, "LED D1", configMINIMAL_STACK_SIZE, & xSemaphore, tskIDLE_PRIORITY + 3, NULL);

    //HANDING OVER THE TASK TO THE SCHEDULER
    vTaskStartScheduler ();

    while(1){}

}

