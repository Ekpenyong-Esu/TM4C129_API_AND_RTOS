
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"


#include <FreeRTOS.h>
#include <task.h>
#include "semphr.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

#define PERIOD pdMS_TO_TICKS(50)

// DECLARING GLOBAL VARIABLE

int missTime = 0;
char stringTime[10];



SemaphoreHandle_t xSemaphore = NULL;



// UART CONFIQURATION

void ConfigureUART(void){

    // Enable PortA GPIO peripheral used by UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable UART0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configuring UART mode
    // U0RX USE FOR RECEIVER
    GPIOPinConfigure(GPIO_PA0_U0RX);

   // U0TX USE FOR TRANSMITTER
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);


    //INITIALIZING UART0 BAUDRATE AND THE FREQUENCY
    UARTStdioConfig(0, 115200, 16000000);
}


void TaskA(void *ptr){

    // BEGIN TASK
    UARTprintf("\r\rTask A started: ");

    // TAKE THE SEMAPHORE
    xSemaphoreTake(xSemaphore, portMAX_DELAY);

    // IF SUCCESFUL PRINT THE SEMAPHORE
    UARTprintf("\r\nTask A semaphore take");
    uint32_t i = 0;
    UARTprintf("\r\nTask A started its workload: ");

    // COUNT TO 500000
    while(i++ < 500000);

    //AFTER COUNTING GIVE THE SEMAPHORE
    UARTprintf("\r\nTask A semaphore give: ");
    xSemaphoreGive(xSemaphore);


    // THEN PRINT TASK ENDED
    UARTprintf("\r\nTask A ended: ");
    vTaskDelete(NULL);


}

void TaskB(void *ptr){
    // DELAY FOR 20s

    vTaskDelay(20);

    // BEGIN TASK
    UARTprintf("\r\nTask B started: ");
    uint32_t i = 0;
    UARTprintf("\r\nTask B started its workload: ");

    //COUNT TO 200000
    while(i++ < 200000);

    //AFTER COUNTING PRINT TASK ENDED
    UARTprintf("\r\nTask B ended: ");
    vTaskDelete(NULL);

}

void TaskC(void *ptr){

    // LOCAL VARAIABLE DECLARATION
        TickType_t xLastWakeTime, xEndTime, xExecTime;
        const TickType_t deadline = 1000;
       //const TickType_t releaseTask = 500;




    // DELAY FOR 10s
       vTaskDelay(10);

    // AFTER 10s, THE TASK BEGIN
    UARTprintf("\r\nTask C started: ");

    // GET THE TIME STAMP
    xLastWakeTime = xTaskGetTickCount();



    // TRY TO TAKE THE SEMAPHOR
    xSemaphoreTake(xSemaphore, portMAX_DELAY);


    // IF SUCCESSFUL, PRINT THE SEMAPHOR
    UARTprintf("\r\nTask C semaphore take: ");
    UARTprintf("\r\nTask C started its workload: ");

    // COUNT TO HUNDRED THOUSAND
    uint32_t i = 0;
    while(i++ < 100000);


    // THEN GIVE THE SEMAPHOR
    UARTprintf("\r\nTask C semaphore give: ");
    xSemaphoreGive(xSemaphore);


    // THEN TASK ENDED
    UARTprintf("\r\nTask C ended: ");

    // GET TIME STAMP
    xEndTime = xTaskGetTickCount();
    xExecTime = (xEndTime-xLastWakeTime)*1000;


    // COUNTING THE MISS TIME
     if(xExecTime >= deadline){

                   // CALCULATING THE MISS TIME
                   missTime = xExecTime;
                   sprintf(stringTime, "%d", missTime);

                   UARTprintf("\r\nTask C has missed the deadline by: ");
                   UARTprintf(stringTime);
                   UARTprintf(" TICKS");

               }
               else{
                   UARTprintf("\r\nTask c has succeeded: ");
               }
    vTaskDelete(NULL);

}


int main(int argc, char **argv) {

    // UART INIT
    ConfigureUART();

    // CREATE SEMAPHORE
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore);


    // CREATING ALL THE TASKS
    xTaskCreate(TaskA, "t1", 100, NULL, 1, NULL);
    xTaskCreate(TaskB, "t2", 100, NULL, 2, NULL);
    xTaskCreate(TaskC, "t3", 100, NULL, 3, NULL);


    // HAND OVER THE TASK TO THE SCHEDULER
    vTaskStartScheduler();

    while(1);
}
